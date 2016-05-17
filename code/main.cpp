// Author(s): Simon, Tom

// C Standard libraries
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <vector>

// Libraries
#include <glew.h>
#include <SDL.h>

//YEP WE'RE DOING A UNITY BUILD HERE
#include "typedefs.h"
#include "math.h"
#include "core.h"
#include "input.h"
#include "file.h"
#include "image.h"
#include "lighting.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "mesh.h"
#include "skybox.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int DESIRED_FPS = 60;

bool HandleEvents(Input* input)
{
	//TODO(Simon): Review which of these only need to be called once
	SDL_Event event;
	bool isRunning = true;
	InputResetMouseScroll(input);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	uint8* keyboardState = (uint8*)SDL_GetKeyboardState(NULL);
	InputInitKeyStates(input, keyboardState);

	int relx, rely;
	SDL_GetRelativeMouseState(&relx, &rely);
	InputSetRelativeMouseMotion(input, relx, rely);

	int x, y;
	SDL_GetMouseState(&x, &y);
	InputSetMousePosition(input, x, y);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				isRunning = false;
			} break;

			case SDL_MOUSEWHEEL:
			{
				InputAddMouseScroll(input, (SDL_MouseWheelEvent*) &event);
			} break;

			case SDL_WINDOWEVENT:
			{
				switch(event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
					{
					} break;
				}
			} break;
		}
	}

	return isRunning;
}

void FirstPersonMovement(Input* input, float deltaTime, Camera* camera)
{
	// NOTE(Tom): First Person Shooter movement
	const float moveSpeed = 3.0f;
	const float angularSpeed = 5.0f;

	Vec3 v = {};

	if (input->keys[SDL_SCANCODE_W])
	{
		v = v + Forward(camera->transform.orientation);
	}
	if (input->keys[SDL_SCANCODE_S])
	{
		v = v + Backward(camera->transform.orientation);
	}
	if (input->keys[SDL_SCANCODE_A])
	{
		v = v + Left(camera->transform.orientation);
	}
	if (input->keys[SDL_SCANCODE_D])
	{
		v = v + Right(camera->transform.orientation);
	}

	if (v.x != 0 || v.y != 0 || v.z != 0)
	{
		v.y = camera->transform.position.y;
		Normalize(&v);
		camera->transform.TranslateTowards(v, moveSpeed * deltaTime);
	}

	camera->transform.Rotate(Vec3::PositiveYAxis(), angularSpeed * input->mouseRelativeX * deltaTime);
	camera->transform.Rotate(Right(camera->transform.orientation), angularSpeed * input->mouseRelativeY * deltaTime);
}

int main(int argc, char* argv[])
{
	// Window initialization
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_Window* window = SDL_CreateWindow(
			"BlackMesa",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			/*SDL_WINDOW_FULLSCREEN_DESKTOP | */SDL_WINDOW_OPENGL
	);
	SDL_GLContext openglContext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW could not initialize!");
		return -1;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Input
	Input input;

	// Graphics

	// Uncomment for wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Set state back to filled polygons
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Back Face Culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	// Depth testing
	glEnable(GL_DEPTH_TEST);

	// Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Game
	Transform cameraTransform;
	cameraTransform.position = { 0.0f, 0.0f, 8.0f };
	cameraTransform.scaling = { 1.0f, 1.0f, 1.0f };
	cameraTransform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, 180.0f);

	Camera camera;
	camera.CreatePerspective(cameraTransform, 60.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

	Transform quadTransform;
	quadTransform.position = { 0.0f, 0.0f, 0.0f };
	quadTransform.scaling = { 1.0f, 1.0f, 1.0f };
	quadTransform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, 0.0f);

	Transform cubeTransform;
	cubeTransform.position = { 0.0f, 0.0f, 4.0f };
	cubeTransform.scaling = { 1.0f, 1.0f, 1.0f };
	cubeTransform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, 0.0f);

	SimpleSpriteMesh quad;
	quad.Create();

	CubeMesh cube;
	cube.Create();

	Texture texture;
	texture.LoadFromFile("../data/textures/orange.bmp");
	// texture.LoadFromFile("../data/textures/foo.dds");
	texture.type = Texture::DIFFUSE;

	MultiTexture multiTexture;
	multiTexture.LoadTextureFromFile("../data/textures/foo.bmp", Texture::DIFFUSE);
	multiTexture.LoadTextureFromFile("../data/textures/foo.dds", Texture::DIFFUSE);

	Material material;
	material.specular = { 0.5f, 0.5f, 0.5f };
	material.shine = 64.0f;

	DirectionalLight dirLight;
	dirLight.direction = { 0.0f, 0.0f, -1.0f };
	dirLight.ambient = { 0.7f, 0.7f, 0.7f };
	dirLight.diffuse = { 0.5f, 0.5f, 0.5f };
	dirLight.specular = { 1.0f, 1.0f, 1.0f };

	Shader shader;
	shader.LoadFromFiles("../data/shaders/phong_vs.glsl", 0, 0, 0, "../data/shaders/phong_fs.glsl");
	PhongShader_Init(&shader);

	Shader skyboxShader;
	skyboxShader.LoadFromFiles("../data/shaders/skyboxVert.glsl", 0, 0, 0, "../data/shaders/skyboxFrag.glsl");
	SkyboxShader_Init(&skyboxShader);
	
	Skybox skybox;
	Skybox_LoadFromFiles(&skybox,
		"../data/textures/entropic_right.dds",
		"../data/textures/entropic_left.dds",
		"../data/textures/entropic_up.dds",
		"../data/textures/entropic_down.dds",
		"../data/textures/entropic_back.dds",
		"../data/textures/entropic_front.dds"
	);
	Skybox_Create(&skybox);

	float deltaTime;
	Uint64 currentTime;
	Uint64 previousTime = SDL_GetPerformanceCounter();
	float tickSize = 1.0f / SDL_GetPerformanceFrequency();
	bool isRunning = true;

	// Update
	while (isRunning)
	{
		currentTime = SDL_GetPerformanceCounter();
		deltaTime = (currentTime - previousTime) * tickSize;

		isRunning = HandleEvents(&input);

		FirstPersonMovement(&input, deltaTime, &camera);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Update();

		shader.Use();
		multiTexture.Use(shader);

		quad.Use();
		PhongShader_Update(&shader, quadTransform, camera);
		PhongShader_UpdateMaterial(&shader, material);
		PhongShader_UpdateLight(&shader, dirLight);
		quad.Render();
		quad.Unuse();
		multiTexture.Unuse();
		
		texture.Use();
		cube.Use();
		PhongShader_Update(&shader, cubeTransform, camera);
		PhongShader_UpdateMaterial(&shader, material);
		PhongShader_UpdateLight(&shader, dirLight);
		cube.Render();
		cube.Unuse();
		texture.Unuse();

		shader.Unuse();

		//Skybox needs to be drawn last
		Skybox_Render(&skybox, &skyboxShader, camera);

		SDL_GL_SwapWindow(window);

		previousTime = currentTime;
	}

	// Shutdown
	shader.Delete();
	quad.Delete();
	cube.Delete();
	SDL_GL_DeleteContext(openglContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

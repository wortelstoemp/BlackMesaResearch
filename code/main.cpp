// Author(s): Simon, Tom 

// C Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Libraries
#include <glew.h>
#include <SDL.h>

//YEP WE'RE DOING A UNITY BUILD HERE
#include "typedefs.h"
#include "math.h"
#include "core.h"
#include "file.h"
#include "image.h"
#include "shader.h"
#include "texture.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int DESIRED_FPS = 30;

const uint points = 4;
const uint floatsPerPoint = 3;

bool HandleEvent(SDL_Event* event)
{
	bool isRunning = true;

	switch(event->type)
	{
		case SDL_QUIT:
		{
			isRunning = false;
		} break;
		case SDL_WINDOWEVENT:
		{
			switch(event->window.event)
			{
				case SDL_WINDOWEVENT_RESIZED:
				{

				} break;
			}
		} break;
	}

	return isRunning;
}

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	
	// Initialization
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	
	int windowFlags = /*SDL_WINDOW_FULLSCREEN_DESKTOP | */SDL_WINDOW_OPENGL;
	SDL_Window* window = SDL_CreateWindow(
			"BlackMesa",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			windowFlags
	);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	glewExperimental = GL_TRUE;
	if (glewInit() != 0)
	{
		printf("glew could not initialize!");
		return -1;
	}
	
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// Mesh stuff
	// TODO(Tom): Make SimpleSprite struct
	GLfloat vertices[] = {
		// Positions		Colors			Texture Coordinates
		0.5, 0.5, 0.0,		1.0, 1.0, 1.0,	1.0, 1.0,	// Top Right
		-0.5, 0.5, 0.0,		1.0, 1.0, 1.0,	0.0, 1.0,	// Top Left
		-0.5, -0.5, 0.0,	1.0, 1.0, 1.0,	0.0, 0.0,	// Bottom Left
		0.5, -0.5, 0.0,		1.0, 1.0, 1.0,	1.0, 0.0	// Bottom Right	
	};
	
	GLuint indices[] = {
		0, 1, 2,	// First Triangle
		2, 3, 0		// Second Triangle
	};
	
	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

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
	
	Transform transform;
	transform.position.X = 0.0f;
	transform.position.Y = 0.0f;
	transform.position.Z = 0.0f;
	transform.scale.X = 1.0f;
	transform.scale.Y = 1.0f;
	transform.scale.Z = 1.0f;
	transform.orientation = FromEuler(180.0f, 0.0f, 0.0f);
	
	Transform cameraTransform;
	cameraTransform.position.X = 0.0f;
	cameraTransform.position.Y = 0.0f;
	cameraTransform.position.Z = 3.0f;
	cameraTransform.scale.X = 1.0f;
	cameraTransform.scale.Y = 1.0f;
	cameraTransform.scale.Z = 1.0f;
	cameraTransform.orientation = FromAxis(0.0f, 1.0f, 0.0f, 180.0f);	
	
	Camera camera;
	camera.CreatePerspective(cameraTransform, 45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
	
	Texture texture;
	texture.Create("../data/textures/foo.bmp");
	//texture.Create("../data/textures/foo.dds");
	
	DefaultShader shader;
	char* vertexSource = ReadFile("../data/shaders/default_vs.glsl");
	char* fragmentSource = ReadFile("../data/shaders/default_fs.glsl");
	shader.Create(vertexSource, 0, 0, 0, fragmentSource);
	FreeFile(vertexSource);
	FreeFile(fragmentSource);
	
	shader.Init();
	
	Uint64 previousTime = SDL_GetPerformanceCounter();
	float tickSize = 1.0f / SDL_GetPerformanceFrequency();
	bool isRunning = true;
	
	// Update
	while (isRunning)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
        {
            isRunning = HandleEvent(&event);    
        }
		
		Uint64 currentTime = SDL_GetPerformanceCounter();
		float deltaTime = (currentTime - previousTime) * tickSize;
		
		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		texture.Bind();
		shader.Use();
		shader.Update(transform, camera, deltaTime);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		texture.Unbind();
		shader.Unuse();
		glBindVertexArray(0);
		
		SDL_GL_SwapWindow(window);	
		
		previousTime = currentTime;
	}

	// Shutdown
	shader.Delete();
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

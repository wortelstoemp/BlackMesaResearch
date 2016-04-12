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
#include "input.h"
#include "file.h"
#include "image.h"
#include "shader.h"
#include "texture.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int DESIRED_FPS = 60;

const uint points = 4;
const uint floatsPerPoint = 3;

// TODO(Tom): Will this simplify a lot?
/*
void UpdateInput_Test(InputSystem* input)
{
    Uint8* keystate = SDL_GetKeyState(NULL);

    //continuous-response keys
    if(keystate[SDLK_LEFT])
    {
    }
    if(keystate[SDLK_RIGHT])
    {
    }
    if(keystate[SDLK_UP])
    {
    }
    if(keystate[SDLK_DOWN])
    {
    }

    //single-hit keys, mouse, and other general SDL events (eg. windowing)
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_MOUSEMOTION:
            break;

            case SDL_QUIT:
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    done = true; //quit
            break;
        }
    }
}
*/

bool UpdateInput(InputSystem* input)
{
	input->Reset();
	
	bool isRunning = true;
	
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				isRunning = false;
			}
			break;
			
			case SDL_KEYDOWN:
			{
				int value = event.key.keysym.scancode;
				input->SetKey(value, true);
				input->SetKeyDown(value, true);
			}
			break;
			
			case SDL_KEYUP:
			{
				int value = event.key.keysym.scancode;
				input->SetKey(value, false);
				input->SetKeyUp(value, true);
			}
			break;
			
			case SDL_WINDOWEVENT:
			{
				switch(event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
					{
					}
					break;
				}
			}
			break;
		}
	}
	
	return isRunning;
}


// bool HandleEvent(InputSystem* input, const SDL_Event& event)
// {
// 	bool isRunning = true;
// 
// 	switch(event.type)
// 	{
// 		case SDL_KEYDOWN:
//         case SDL_KEYUP:
//         {
//             SDL_Keycode keyCode = event.key.keysym.sym;
//             bool isDown = (event.key.state == SDL_PRESSED);
//             bool wasDown = false;
//             if (event.key.state == SDL_RELEASED)
//             {
//                 wasDown = true;
//             }
//             else if (event.key.repeat != 0)
//             {
//                 wasDown = true;
//             }
//             
//             if (event.key.repeat == 0)
//             {
//                 if(keyCode == SDLK_UP)
//                 {
//                     if(isDown)
//                     {
// 						input->SetKey(InputSystem::KEY_UP, true);
// 						input->SetKeyDown(InputSystem::KEY_UP, true);
//                     }
// 					
//                     if(wasDown)
//                     {
// 						input->SetKey(InputSystem::KEY_UP, false);
// 						input->SetKeyUp(InputSystem::KEY_UP, true);
//                     }
//                 }
//                 else if(keyCode == SDLK_LEFT)
//                 {
// 					printf("Left: ");
//                     if(isDown)
//                     {
//                         printf("isDown ");
//                     }
//                     if(wasDown)
//                     {
//                         printf("wasDown");
//                     }
//                     printf("\n");
//                 }
//                 else if(keyCode == SDLK_DOWN)
//                 {
//                 }
//                 else if(keyCode == SDLK_RIGHT)
//                 {
//                 }
//                 else if(keyCode == SDLK_f)
//                 {
//                     printf("F: ");
//                     if(isDown)
//                     {
//                         printf("isDown ");
//                     }
//                     if(wasDown)
//                     {
//                         printf("wasDown");
//                     }
//                     printf("\n");
//                 }
//                 else if(keyCode == SDLK_l)
//                 {
//                 }
//             }
// 		}
// 		break;
// 		
// 		case SDL_QUIT:
// 		{
// 			isRunning = false;
// 		}
// 		break;
// 		
// 		case SDL_WINDOWEVENT:
// 		{
// 			switch(event.window.event)
// 			{
// 				case SDL_WINDOWEVENT_RESIZED:
// 				{
// 
// 				}
// 				break;
// 			}
// 		}
// 		break;
// 	}
// 
// 	return isRunning;
// }

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	
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
	
	// Input initialization
	InputSystem input;
	
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
	texture.CreateFromFile("../data/textures/foo.bmp");
	//texture.CreateFromFile("../data/textures/foo.dds");
	
	DefaultShader shader;
	shader.CreateFromFiles("../data/shaders/default_vs.glsl", 0, 0, 0, "../data/shaders/default_fs.glsl");
	shader.Init();
	
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
		
		isRunning = UpdateInput(&input);
		if (input.IsKey(InputSystem::KEY_UP))
		{
			printf("Pressed key up!\n");
		}
		
		if (input.IsKeyUp(InputSystem::KEY_UP))
		{
			printf("Released key up!\n");
		}	
		
		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		camera.Update();
		camera.transform.Rotate(0.0f, 5.0f * deltaTime, 0.0f);
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
	SDL_GL_DeleteContext(openglContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

// Author(s): Simon, Tom

// C Standard libraries
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// C++ Standard libraries
#include <vector>

// Libraries
#include <glew.h>
#include <SDL.h>

//YEP WE'RE DOING A UNITY BUILD HERE
#include "typedefs.h"
#include "math.h"
#include "core.h"
#include "intersection.h"
#include "input.h"
#include "file.h"
#include "image.h"
#include "lighting.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "skybox.h"
#include "game.h"

const int32 SCREEN_WIDTH = 1600;
const int32 SCREEN_HEIGHT = 900;
const int32 DESIRED_FPS = 60;

SDL_GLContext openglContext;
SDL_Window* window;

bool InitGL()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
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

	window = SDL_CreateWindow(
			"BlackMesa",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			/*SDL_WINDOW_FULLSCREEN_DESKTOP | */SDL_WINDOW_OPENGL
	);
	openglContext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW could not initialize!");
		return false;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Depth testing
	glEnable(GL_DEPTH_TEST);

	// Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Uncomment for wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Set state back to filled polygons
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Back Face Culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	return true;
}

int32 main(int32 argc, char* argv[])
{
	if (!InitGL())
	{
		return -1;
	}

	World world = {};

	InitGame(&world);

	Input input = {};

	float deltaTime;
	Uint64 currentTime;
	Uint64 previousTime = SDL_GetPerformanceCounter();
	float tickSize = 1.0f / SDL_GetPerformanceFrequency();
	bool isRunning = true;

	while (isRunning)
	{
		currentTime = SDL_GetPerformanceCounter();
		deltaTime = (currentTime - previousTime) * tickSize;

		isRunning = HandleEvents(&input);

		if (!isRunning)
		{
			break;
		}
		GameUpdateAndRender(&input, deltaTime, &world);

		SDL_GL_SwapWindow(window);

		previousTime = currentTime;
	}

	SDL_GL_DeleteContext(openglContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

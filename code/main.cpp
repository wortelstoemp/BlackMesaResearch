#include <windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <stdio.h>
#include <SDL.h>
#include <string.h>

typedef unsigned int uint;

#include "gltk.h"
#include "file.h"
#include "shader.h"

// Author(s): Simon, Tom 


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
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

void GameUpdateAndRender(SDL_Window* window, float deltaTime)
{
	glClearColor(1, 0, 1, 1);

	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat square[points][floatsPerPoint] =
	{
		{ -0.5,  0.5,  0.5 }, // Top left
		{  0.5,  0.5,  0.5 }, // Top right
		{  0.5, -0.5,  0.5 }, // Bottom right
		{ -0.5, -0.5,  0.5 }, // Bottom left
	};

 	GLuint vbo[1], vao[1];

 	uint positionAttributeIndex = 0;
 	glGenBuffers(1, vbo);
 	glGenVertexArrays(1, vao);
	//glBufferData(GL_ARRAY_BUFFER, vbo[0]);
		 
	uint sizeInBytes = (points * floatsPerPoint) * sizeof(GLfloat);
	glBufferData(GL_ARRAY_BUFFER, sizeInBytes, square, GL_STATIC_DRAW);

	glBindVertexArray(vao[0]);
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(positionAttributeIndex);

	SDL_GL_SwapWindow(window);
}

int main(int argc, char* argv[])
{
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
	
	Uint64 previousTime = SDL_GetPerformanceCounter();
	float tickSize = 1.0f / SDL_GetPerformanceFrequency();
	
	// Update
	bool isRunning = true;
	while (isRunning)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
        {
            isRunning = HandleEvent(&event);    
        }
		
		Uint64 currentTime = SDL_GetPerformanceCounter();
		float deltaTime = (currentTime - previousTime) * tickSize;
		GameUpdateAndRender(window, deltaTime);
		previousTime = currentTime;
	}

	// Shutdown
	SDL_Quit();

	return 0;
}

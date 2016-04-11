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

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
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
	(void) deltaTime;
	
	
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
	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
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
	
	ShaderProgram shader;
	char* vertexSource = ReadFile("../data/shaders/simpleVert.glsl");
	char* fragmentSource = ReadFile("../data/shaders/simpleFrag.glsl");
	shader.Create(vertexSource, 0, 0, 0, fragmentSource);
	FreeFile(vertexSource);
	FreeFile(fragmentSource);	
	
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
		glClear(GL_COLOR_BUFFER_BIT);
	
		// Create Vertex Array Object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Create a Vertex Buffer Object and copy the vertex data to it
		GLuint vbo;
		glGenBuffers(1, &vbo);

		GLfloat verticesOld[] = {
			0.0f,  0.5f,
			0.5f, -0.5f,
			-0.5f, -0.5f
		};

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOld), verticesOld, GL_STATIC_DRAW);

		shader.Use();

		// Specify the layout of the vertex data
		GLint posAttrib = glGetAttribLocation(shader.shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Draw a triangle from the 3 verticesOld
		glDrawArrays(GL_TRIANGLES, 0, 3);
	
		SDL_GL_SwapWindow(window);	
		
		previousTime = currentTime;
	}

	// Shutdown
	shader.Delete();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

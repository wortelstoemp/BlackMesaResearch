#pragma once

// Author(s): Tom

struct World
{
	Camera camera;
	Skybox skybox;
};

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

void InitGame(World* world)
{
	Transform cameraTransform;
	cameraTransform.position = { 0.0f, 0.0f, 8.0f };
	cameraTransform.scaling = { 1.0f, 1.0f, 1.0f };
	cameraTransform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, 180.0f);

	int32 viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float width = viewport[2];
	float height = viewport[3];

	world->camera.CreatePerspective(cameraTransform, 60.0f, width / height, 0.1f, 100.0f);

	SkyboxLoadTextureFromFiles(&world->skybox,
		"../data/textures/eve_right.dds",
		"../data/textures/eve_left.dds",
		"../data/textures/eve_up.dds",
		"../data/textures/eve_down.dds",
		"../data/textures/eve_back.dds",
		"../data/textures/eve_front.dds"
	);
	SkyboxCreate(&world->skybox);
}

void GameUpdateAndRender(Input* input, float deltaTime, World* world)
{
	FirstPersonMovement(input, deltaTime, &world->camera);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	world->camera.Update();

	//NOTE(Simon): Skybox needs to be drawn last
	SkyboxRender(&world->skybox, world->camera);
}

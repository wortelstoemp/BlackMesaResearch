#pragma once


struct Entity
{
	Transform transform;
	Mesh mesh;
	Texture texture;
	Material material;
	Shader shader;
	DirectionalLight directionalLight;
	std::vector<Entity> entities;
	void (*Behaviour) (Entity*, Input*);
};

struct World
{
	Camera camera;
	Skybox skybox;
	DirectionalLight directionalLight;
	std::vector<Entity> entities;
};

void AddEntityToWorld(World* world, Entity* entity)
{
	world->entities.push_back(*entity);
}

void AddEntityToWorld(Entity* parentEntity, Entity* entity)
{
	parentEntity->entities.push_back(*entity);
}

bool HandleEvents(Input* input)
{
	//TODO(Simon): Review which of these only need to be called once
	SDL_Event event;
	bool isRunning = true;
	InputResetMouseScroll(input);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	uint8* keyboardState = (uint8*)SDL_GetKeyboardState(NULL);
	InputInitKeyStates(input, keyboardState);

	int32 relx, rely;
	SDL_GetRelativeMouseState(&relx, &rely);
	InputSetRelativeMouseMotion(input, relx, rely);

	int32 x, y;
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

void FirstPersonMovement(Input* input, Camera* camera)
{
	// NOTE(Tom): First Person Shooter movement
	float moveSpeed = 3.0f;
	float angularSpeed = 5.0f;

	Vec3 v = {};

	if(input->keys[SDL_SCANCODE_LSHIFT])
	{
		moveSpeed *= 10.0f;
	}

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
		camera->transform.TranslateTowards(v, moveSpeed * input->deltaTime);
	}

	camera->transform.Rotate(Vec3::Up(), angularSpeed * input->mouseRelativeX * input->deltaTime);
	camera->transform.Rotate(Right(camera->transform.orientation), angularSpeed * input->mouseRelativeY * input->deltaTime);
}

void EarthBehaviour(Entity* earth, Input* input)
{
	earth->transform.Rotate(Vec3::Up(), 0.05f);
	earth->transform.position.x = 1000 * cos(input->totalTime / 100.0f);
	earth->transform.position.z = 1000 * sin(input->totalTime / 100.0f);
}

void InitGame(World* world)
{
	Transform cameraTransform = CreateTransform();
	cameraTransform.position = { 0.0f, 0.0f, 8.0f };
	cameraTransform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, 180.0f);

	int32 viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float width = viewport[2];
	float height = viewport[3];

	world->camera.CreatePerspective(cameraTransform, 60.0f, width / height, 0.1f, 10000.0f);

	SkyboxLoadTextureFromFiles(&world->skybox,
		"../data/textures/eve_right.dds",
		"../data/textures/eve_left.dds",
		"../data/textures/eve_up.dds",
		"../data/textures/eve_down.dds",
		"../data/textures/eve_back.dds",
		"../data/textures/eve_front.dds"
	);
	SkyboxCreate(&world->skybox);

	Material material;
	material.specular = { 0.5f, 0.5f, 0.5f };
	material.shine = 64.0f;

	Shader shader;
	shader.LoadFromFiles("../data/shaders/phong_vs.glsl", 0, 0, 0, "../data/shaders/phong_fs.glsl");
	PhongShader_Init(&shader);

	Entity quad = {};
	quad.transform = CreateTransform();
	quad.mesh = Mesh_CreateFromFile("../data/meshes/quad.qvm");
	quad.texture.LoadFromFile("../data/textures/orange.bmp");
	quad.texture.type = Texture::DIFFUSE;
	quad.material = material;
	quad.shader = shader;
	AddEntityToWorld(world, &quad);

	Entity cube = {};
	cube.transform = CreateTransform();
	cube.transform.position.z = 4.0f;
	cube.mesh = Mesh_CreateFromFile("../data/meshes/cube.qvm");
	cube.texture.LoadFromFile("../data/textures/orange.bmp");
	cube.texture.type = Texture::DIFFUSE;
	cube.material = material;
	cube.shader = shader;
	AddEntityToWorld(world, &cube);

	Entity robot = {};
	robot.transform = CreateTransform();
	robot.transform.position = { 4.0f, 0.0f, 6.0f };
	robot.transform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, 45.0f);
	robot.mesh = Mesh_CreateFromFile("../data/meshes/robot.obj");
	robot.texture.LoadFromFile("../data/textures/orange.bmp");
	robot.texture.type = Texture::DIFFUSE;
	robot.material = material;
	robot.shader = shader;
	AddEntityToWorld(world, &robot);

	Entity earth = {};
	earth.transform = CreateTransform();
	earth.transform.scale = {100, 100, 100};
	earth.transform.position.x = 200;
	earth.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
	earth.texture.LoadFromFile("../data/textures/4096_earth.dds");
	earth.texture.type = Texture::DIFFUSE;
	earth.material = material;
	earth.shader = shader;
	earth.Behaviour = &EarthBehaviour;
	AddEntityToWorld(world, &earth);

	DirectionalLight dirLight;
	dirLight.direction = { 0.0f, 0.0f, -1.0f };
	dirLight.ambient = { 0.7f, 0.7f, 0.7f };
	dirLight.diffuse = { 0.5f, 0.5f, 0.5f };
	dirLight.specular = { 0.70f, 0.58f, 0.38f };
	world->directionalLight = dirLight;
}

void GameUpdateAndRender(Input* input, World* world)
{
	FirstPersonMovement(input, &world->camera);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int32 numEntities = world->entities.size();

	for (int32 i = 0; i < numEntities; i++)
	{
		Entity* currentEntity = &world->entities[i];
		if(currentEntity->Behaviour)
		{
			currentEntity->Behaviour(currentEntity, input);
		}
	}

	world->camera.Update();

	for (int32 i = 0; i < numEntities; i++)
	{
		Entity* currentEntity = &world->entities[i];
		currentEntity->shader.Use();
		currentEntity->texture.Use();

		PhongShader_Update(&currentEntity->shader, currentEntity->transform, world->camera);
		PhongShader_UpdateMaterial(&currentEntity->shader, currentEntity->material);
		PhongShader_UpdateLight(&currentEntity->shader, world->directionalLight);
		Mesh_Render(&currentEntity->mesh);

		currentEntity->texture.Unuse();
		currentEntity->shader.Unuse();
	}

	//NOTE(Simon): Skybox needs to be drawn last
	SkyboxRender(&world->skybox, world->camera);
}

#pragma once

// Author(s): Simon, Tom

struct Entity
{
	uint64 name;
	Transform transform;
	AABB boundingBox;
	Mesh mesh;
	Texture texture;
	Material material;
	Shader shader;
	DirectionalLight directionalLight;
	std::vector<Entity> entities;
	void (*Behaviour) (Entity*, Input*);
};

struct RenderQueue
{
	std::vector<Entity*> entities;
};

struct World
{
	Camera camera;
	Skybox skybox;
	DirectionalLight directionalLight;
	Spotlight spotlight;
	std::vector<Entity> entities;
	RenderQueue renderQueue;
};

void AddEntityToWorld(World* world, Entity* entity)
{
	world->entities.push_back(*entity);
}

void AddEntityToWorld(Entity* parentEntity, Entity* entity)
{
	parentEntity->entities.push_back(*entity);
}

Entity* GetEntityByName(World* world, char* name)
{
	uint64 hash = Hash(name);
	int32 numEntities = world->entities.size();
	for (int i = 0; i < numEntities; i++)
	{
		if (world->entities[i].name == hash)
		{
			return &world->entities[i];
		}
	}

	return 0;
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

	for (int i = 0; i < INPUT_NUM_MOUSEBUTTONS; i++)
	{
		input->mouseButtonsDown[i] = 0;
		input->mouseButtonsUp[i] = 0;
	}

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

			case SDL_MOUSEBUTTONDOWN:
			{
				int button = event.button.button;
				input->mouseButtons[button] = 1;
				input->mouseButtonsDown[button] = 1;
			} break;

			case SDL_MOUSEBUTTONUP:
			{
				int button = event.button.button;
				input->mouseButtons[button] = 0;
				input->mouseButtonsUp[button] = 1;
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

Ray CalculatePickingRayFromCamera(const Camera& camera)
{
	Ray result;
	result.origin = camera.transform.position;
	result.direction = Forward(camera.transform.orientation);
	return result;
}

void FirstPersonMovement(Input* input, World* world)
{
	// NOTE(Tom): First Person Shooter movement
	float moveSpeed = 15.0f;
	float angularSpeed = 5.0f;

	Camera* camera = &world->camera;

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
	if(input->keys[SDL_SCANCODE_LCTRL])
	{
		v = v + Vec3::Up();
	}
	if(input->keys[SDL_SCANCODE_LALT])
	{
		v = v + Vec3::Down();
	}

	if (v.x != 0 || v.y != 0 || v.z != 0)
	{
		Normalize(&v);
		camera->transform.TranslateTowards(v, moveSpeed * input->deltaTime);
	}

	Entity* cockpit = GetEntityByName(world, "cockpit");
	Vec3 cockpitOffset = {1.112664f, 4.151459f, 1.131007f};

	cockpit->transform.position = camera->transform.position;

	camera->transform.Rotate(Vec3::Up(), angularSpeed * input->mouseRelativeX * input->deltaTime);
	camera->transform.Rotate(Right(camera->transform.orientation), angularSpeed * input->mouseRelativeY * input->deltaTime);

	cockpit->transform.orientation = Conjugate(camera->transform.orientation);
	cockpit->transform.position = cockpit->transform.position;
}

void GlobalHotkeys(Input* input, World* world)
{
	if(input->keys[SDL_SCANCODE_P])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if(input->keys[SDL_SCANCODE_O])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


void PlanetRotation(Entity* planet, Input* input, float centerDistance, float rotationTime, float revolutionTime)
{
	planet->transform.Rotate(Vec3::Up(), 1 / (rotationTime * input->deltaTime));

	planet->transform.position.x = (centerDistance / 3e6) * cos(input->totalTime / revolutionTime * 10);
	planet->transform.position.z = (centerDistance / 3e6) * sin(input->totalTime / revolutionTime * 10);
}

void SunBehaviour(Entity* sun, Input* input)
{
	//NOTE(Simon): Weird values to prevent division by zero in PlanetRotation.
	PlanetRotation(sun, input, 0, 489.4f, 1e6);
}

void MercuryBehaviour(Entity* mercury, Input* input)
{
	PlanetRotation(mercury, input, 69.8e6, 1080, 8.797f);
}

void VenusBehaviour(Entity* venus, Input* input)
{
	PlanetRotation(venus, input, 108.9e6, -4860, 22.47f);
}

void EarthBehaviour(Entity* earth, Input* input)
{
	PlanetRotation(earth, input, 152.1e6, 20, 36.526f);
}

void MarsBehaviour(Entity* mars, Input* input)
{
	PlanetRotation(mars, input, 249.2e6, 20, 68.698f);
}

void JupiterBehaviour(Entity* jupiter, Input* input)
{
	PlanetRotation(jupiter, input, 816.6e6, 8.25f, 433.198f);
}

void SaturnBehaviour(Entity* saturn, Input* input)
{
	PlanetRotation(saturn, input, 1514.5e6, 8.75f, 1076.056f);
}

void UranusBehaviour(Entity* uranus, Input* input)
{
	PlanetRotation(uranus, input, 3003.6e6, 14.4f, 3068.55f);
}

void NeptuneBehaviour(Entity* neptune, Input* input)
{
	PlanetRotation(neptune, input, 4545.7e6, 13.424f, 6019.12f);
}

void PickEntity(Input* input, World* world)
{
	if (input->mouseButtons[INPUT_MOUSE_BUTTON_LEFT])
	{
		int32 numEntities = world->entities.size();
		Ray pickingRay = CalculatePickingRayFromCamera(world->camera);
		Entity* nearestEntity = NULL;
		float nearestDistance = FLT_MAX;

		for (int32 i = 0; i < numEntities; i++)
		{
			Entity* currentEntity = &world->entities[i];

			//ignore if no boundingBox
			if (currentEntity->boundingBox.min.x != 0 ||
				currentEntity->boundingBox.min.y != 0 ||
				currentEntity->boundingBox.min.z != 0 ||
				currentEntity->boundingBox.max.x != 0 ||
				currentEntity->boundingBox.max.y != 0 ||
				currentEntity->boundingBox.max.z != 0)
			{
				IntersectionData intersectionData =	IntersectRayOBB(pickingRay, currentEntity->boundingBox, currentEntity->transform);

				if (intersectionData.intersects && intersectionData.distance < nearestDistance)
				{
					nearestDistance = intersectionData.distance;
					nearestEntity = currentEntity;
				}
			}
		}

		if (nearestEntity)
		{
			Vec3 direction = Forward(world->camera.transform.orientation);
			const float speed = 15.0f;
			nearestEntity->transform.TranslateTowards(direction, speed * input->deltaTime);
		}
	}
}

void AddToRenderQueue(RenderQueue* queue, Entity* entity)
{
	queue->entities.push_back(entity);
}

bool RenderQueueSort(Entity* a, Entity* b)
{
	//Sort by shader for batching;
	return a->shader.program < b->shader.program;
}

void DrawRenderQueue(RenderQueue* queue, World* world)
{
	std::sort(queue->entities.begin(), queue->entities.end(), RenderQueueSort);

	GLuint currentShader = -1;
	int32 numEntities = queue->entities.size();

	for (int32 i = 0; i < numEntities; i++)
	{
		Entity* entity = queue->entities[i];

		if (currentShader != entity->shader.program)
		{
			UseShader(entity->shader.program);
			currentShader = entity->shader.program;
		}

		entity->texture.Use();

		switch(entity->shader.type)
		{
			case ShaderType_Phong:
			{
				PhongShader_Update(&entity->shader, entity->transform, world->camera);
				PhongShader_UpdateMaterial(&entity->shader, entity->material);
				PhongShader_UpdateLight(&entity->shader, world->directionalLight, world->spotlight);
			} break;
			case ShaderType_Default:
			{
				DefaultShader_Update(&entity->shader, entity->transform, world->camera);
			} break;
			default:
			{
				Assert(0);
				printf("You forgot to set the ShaderType enum");
			}
		}
		Mesh_Render(&entity->mesh);

		entity->texture.Unuse();
	}

	UnuseShader();

	queue->entities.clear();
}

void InitGame(World* world)
{
	Transform cameraTransform = CreateTransform();
	cameraTransform.position = { 11.112664f, 14.151459f, 7.131007f };
	cameraTransform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, -45.0f);

	int32 viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float width = viewport[2];
	float height = viewport[3];

	world->camera.CreatePerspective(cameraTransform, 60.0f, width / height, 0.25f, 10000.0f);

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

	Texture fighterTexture;
	fighterTexture.LoadFromFile("../data/textures/fighter.dds");
	//NOTE(Simon): Offset by that vector for easy positioning in cockpit
	Mesh fighterMesh = Mesh_CreateFromFile("../data/meshes/fighter.obj", {0, -3.85f * 1 / 0.025f, -1.6f * 1 / 0.025f});

	Entity cockpit = {};
		cockpit.name = Hash("cockpit");
		cockpit.transform = CreateTransform();
		cockpit.transform.position = { 10.0f, 10.0f, 6.0f };
		cockpit.transform.scale = {0.025f, 0.025f, 0.025f};
		cockpit.transform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, 45.0f);
		cockpit.mesh = fighterMesh;
		cockpit.texture = fighterTexture;
		cockpit.texture.type = Texture::DIFFUSE;
		cockpit.material = material;
		cockpit.shader = shader;
	AddEntityToWorld(world, &cockpit);

	Entity fighter = {};
		fighter.name = Hash("fighter");
		fighter.transform = CreateTransform();
		fighter.transform.position = { 10.0f, -4.0f, 6.0f };
		fighter.transform.scale = {0.025f, 0.025f, 0.025f};
		fighter.transform.orientation = QuaternionFromAxis(0.0f, 1.0f, 0.0f, 45.0f);
		fighter.boundingBox.min = { -5.0f, -5.0f, -5.0f };
		fighter.boundingBox.max = { 5.0f, 5.0f, 5.0f };
		fighter.mesh = fighterMesh;
		fighter.texture = fighterTexture;
		fighter.texture.type = Texture::DIFFUSE;
		fighter.material = material;
		fighter.shader = shader;
	AddEntityToWorld(world, &fighter);

	Entity sun = {};
		sun.name = Hash("sun");
		sun.transform = CreateTransform();
		sun.transform.scale = {log10f(1393000), log10f(1393000), log10f(1393000)};
		sun.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		sun.texture.LoadFromFile("../data/textures/sun.dds");
		sun.texture.type = Texture::DIFFUSE;
		sun.material = material;
		sun.shader = shader;
		sun.Behaviour = &SunBehaviour;
	AddEntityToWorld(world, &sun);

	Entity mercury = {};
		mercury.name = Hash("mercury");
		mercury.transform = CreateTransform();
		mercury.transform.scale = {log10f(4880), log10f(4880), log10f(4880)};
		mercury.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		mercury.texture.LoadFromFile("../data/textures/mercury.dds");
		mercury.texture.type = Texture::DIFFUSE;
		mercury.material = material;
		mercury.shader = shader;
		mercury.Behaviour = &MercuryBehaviour;
	AddEntityToWorld(world, &mercury);

	Entity venus = {};
		venus.name = Hash("venus");
		venus.transform = CreateTransform();
		venus.transform.scale = {log10f(12104), log10f(12104), log10f(12104)};
		venus.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		venus.texture.LoadFromFile("../data/textures/venus.dds");
		venus.texture.type = Texture::DIFFUSE;
		venus.material = material;
		venus.shader = shader;
		venus.Behaviour = &VenusBehaviour;
	AddEntityToWorld(world, &venus);

	Entity earth = {};
		earth.name = Hash("earth");
		earth.transform = CreateTransform();
		earth.transform.scale = {log10f(12742), log10f(12742), log10f(12742)};
		earth.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		earth.texture.LoadFromFile("../data/textures/earth.dds");
		earth.texture.type = Texture::DIFFUSE;
		earth.material = material;
		earth.shader = shader;
		earth.Behaviour = &EarthBehaviour;
	AddEntityToWorld(world, &earth);

	Entity mars = {};
		mars.name = Hash("mars");
		mars.transform = CreateTransform();
		mars.transform.scale = {log10f(6780), log10f(6780), log10f(6780)};
		mars.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		mars.texture.LoadFromFile("../data/textures/mars.dds");
		mars.texture.type = Texture::DIFFUSE;
		mars.material = material;
		mars.shader = shader;
		mars.Behaviour = &MarsBehaviour;
	AddEntityToWorld(world, &mars);

	Entity jupiter = {};
		jupiter.name = Hash("jupiter");
		jupiter.transform = CreateTransform();
		jupiter.transform.scale = {log10f(139822), log10f(139822), log10f(139822)};
		jupiter.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		jupiter.texture.LoadFromFile("../data/textures/jupiter.dds");
		jupiter.texture.type = Texture::DIFFUSE;
		jupiter.material = material;
		jupiter.shader = shader;
		jupiter.Behaviour = &JupiterBehaviour;
	AddEntityToWorld(world, &jupiter);

	Entity saturn = {};
		saturn.name = Hash("saturn");
		saturn.transform = CreateTransform();
		saturn.transform.scale = {log10f(116464), log10f(116464), log10f(116464)};
		saturn.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		saturn.texture.LoadFromFile("../data/textures/saturn.dds");
		saturn.texture.type = Texture::DIFFUSE;
		saturn.material = material;
		saturn.shader = shader;
		saturn.Behaviour = &SaturnBehaviour;
	AddEntityToWorld(world, &saturn);

	Entity uranus = {};
		uranus.name = Hash("uranus");
		uranus.transform = CreateTransform();
		uranus.transform.scale = {log10f(50724), log10f(50724), log10f(50724)};
		uranus.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		uranus.texture.LoadFromFile("../data/textures/uranus.dds");
		uranus.texture.type = Texture::DIFFUSE;
		uranus.material = material;
		uranus.shader = shader;
		uranus.Behaviour = &UranusBehaviour;
	AddEntityToWorld(world, &uranus);

	Entity neptune = {};
		neptune.name = Hash("neptune");
		neptune.transform = CreateTransform();
		neptune.transform.scale = {log10f(49248), log10f(49248), log10f(49248)};
		neptune.mesh = Mesh_CreateFromFile("../data/meshes/sphere.obj");
		neptune.texture.LoadFromFile("../data/textures/neptune.dds");
		neptune.texture.type = Texture::DIFFUSE;
		neptune.material = material;
		neptune.shader = shader;
		neptune.Behaviour = &NeptuneBehaviour;
	AddEntityToWorld(world, &neptune);

	DirectionalLight dirLight = {};
		dirLight.direction = { 0.0f, 0.0f, -1.0f };
		dirLight.ambient = { 0.5f, 0.5f, 0.5f };
		dirLight.diffuse = { 0.5f, 0.5f, 0.5f };
		dirLight.specular = { 0.70f, 0.58f, 0.38f };
	world->directionalLight = dirLight;

	Spotlight spotlight = {};
		spotlight.direction = Vec3::Forward();
		spotlight.cutOff = cos(Rad(12.5));
		spotlight.outerCutOff = cos(Rad(17.5));
		spotlight.constant = 1.0f;
		spotlight.linear = 0.01f;
		spotlight.quadratic = 0.0032f;
		spotlight.ambient = {0.1f, 0.1f, 0.1f};
		spotlight.diffuse = {0.8f, 0.8f, 0.8f};
		spotlight.specular = {1.0f, 1.0f, 1.0f};
	world->spotlight = spotlight;
}

void GameUpdateAndRender(Input* input, World* world)
{
	// Update
	FirstPersonMovement(input, world);
	GlobalHotkeys(input, world);
	world->camera.Update();

	world->spotlight.position = world->camera.transform.position;
	world->spotlight.direction = Forward(world->camera.transform.orientation);

	int32 numEntities = world->entities.size();

	PickEntity(input, world);

	for (int32 i = 0; i < numEntities; i++)
	{
		Entity* currentEntity = &world->entities[i];
		if(currentEntity->Behaviour)
		{
			currentEntity->Behaviour(currentEntity, input);
		}
	}

	// Render
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int32 i = 0; i < numEntities; i++)
	{
		AddToRenderQueue(&world->renderQueue, &world->entities[i]);
	}

	DrawRenderQueue(&world->renderQueue, world);

	//NOTE(Simon): Skybox needs to be drawn last
	SkyboxRender(&world->skybox, world->camera);
}

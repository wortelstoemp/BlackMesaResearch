#pragma once

// Author(s): Tom
// Currently just a draft

struct Scene
{
	Camera mainCamera;
};

struct Game
{
	Scene currentScene;
	// Later on more scenes can be queued in some cache datastructure
	bool isRunning;
};
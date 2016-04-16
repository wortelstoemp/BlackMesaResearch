#pragma once

// Author(s): Tom
// Currently just a draft

struct Scene
{
	Camera MainCamera;
};

struct Game
{
	Scene CurrentScene;
	// Later on more scenes can be queued in some cache datastructure
	bool IsRunning;
};
#pragma once

// Author(s): Tom

struct AmbientLight
{
	Vec3 color;
	float intensity;	
};

struct DirectionalLight
{
	Vec3 direction;
	float diffuseIntensity;
};
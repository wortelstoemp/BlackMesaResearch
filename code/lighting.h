#pragma once

// Author(s): Tom

struct Attenuation
{
	float constant;
	float linear;
	float exponential;
};

struct Material
{
	// TODO: Color
	float specularIntensity;
	float specularPower;
};

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
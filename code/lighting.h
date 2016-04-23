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
	Vec3 specular;    
	float shine;
};

struct DirectionalLight
{
	Vec3 direction;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
};
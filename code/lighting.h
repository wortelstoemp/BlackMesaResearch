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
// 	Vec3 ambient;
// 	Vec3 diffuse;
	Vec3 specular;    
	float shine;
};

struct Light
{
	Vec3 position;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
};
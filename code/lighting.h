#pragma once

// Author(s): Tom

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

struct Attenuation
{
	float constant;
	float linear;
	float quadratic;
};

struct PointLight
{
	Attenuation attenuation;
	Vec3 position;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
};

struct SpotLight
{
	Attenuation attenuation;
	Vec3 position;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 direction;
	float cutOff;	
};
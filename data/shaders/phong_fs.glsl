#version 400 core

in vec3 fragColor;
in vec2 fragUV;

out vec4 outColor;

struct DirectionalLight
{
	vec3 Color;
	float Ambient;	
};

uniform DirectionalLight directionalLight;
uniform sampler2D diffuseTexture;

void main()
{
	outColor = texture(diffuseTexture, fragUV) * vec4(fragColor, 1.0) * vec4(directionalLight.Color, 1.0f) * directionalLight.Ambient;
}
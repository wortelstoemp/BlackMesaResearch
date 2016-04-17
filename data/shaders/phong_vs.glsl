#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 mvp;
uniform mat4 model;

out vec3 vertexWorldPosition;
out vec3 vertexNormal;
out vec2 vertexUV;

void main()
{
	gl_Position = mvp * vec4(position, 1.0f);
	vertexWorldPosition = (model * vec4(position, 1.0f)).xyz;
	vertexNormal = (model * vec4(normal, 0.0f)).xyz;
	vertexUV = uv;
}
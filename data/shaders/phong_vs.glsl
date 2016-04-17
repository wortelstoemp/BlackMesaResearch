#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 mvp;

out vec3 fragNormal;
out vec2 fragUV;

void main()
{
	gl_Position = mvp * vec4(position.x, position.y, position.z, 1.0f);
	fragNormal = (model * vec4(normal, 0.0f)).xyz;
	fragUV = uv;
}
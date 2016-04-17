#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 mvp;
uniform mat4 model;

out vec3 fragNormal;
out vec2 fragUV;

void main()
{
	gl_Position = mvp * vec4(position, 1.0f);
	fragNormal = vec3(model * vec4(normal, 0.0f));
	fragUV = uv;
}
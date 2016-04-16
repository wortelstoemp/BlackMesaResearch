#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;

uniform mat4 transform;

out vec3 fragColor;
out vec2 fragUV;

void main()
{
	gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);
	fragColor = color;
	fragUV = uv;
}
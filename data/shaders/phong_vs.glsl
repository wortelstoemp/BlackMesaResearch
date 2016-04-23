#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 vertexNormal;
out vec3 vertexWorldPosition;
out vec2 vertexUV;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    gl_Position = mvp * vec4(position, 1.0f);
    vertexWorldPosition = (model * vec4(position, 1.0f)).xyz;
    vertexNormal = mat3(transpose(inverse(model))) * normal; 
	vertexUV = uv;	
} 
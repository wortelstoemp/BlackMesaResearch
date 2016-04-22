#version 400 core

in vec2 fragUV;

out vec4 outColor;

uniform sampler2D diffuseTexture;

void main()
{
	outColor = texture2D(diffuseTexture, fragUV) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
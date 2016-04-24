#version 400 core

in vec2 fragUV;

out vec4 outColor;

uniform sampler2D textureDiffuse1;

void main()
{
	outColor = texture(textureDiffuse1, fragUV) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
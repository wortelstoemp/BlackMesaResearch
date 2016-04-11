#version 400 core

in vec3 fragColor;
in vec2 fragUV;

out vec4 outColor;

uniform sampler2D appTexture;

void main()
{
	outColor = texture(appTexture, fragUV) * vec4(fragColor, 1.0);
}
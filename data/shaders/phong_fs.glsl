#version 400 core

in vec3 fragNormal;
in vec2 fragUV;

out vec4 outColor;

struct AmbientLight
{
	vec3 color;
	float intensity;	
};

struct DirectionalLight
{
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D diffuseTexture;
uniform AmbientLight ambientLight;
uniform DirectionalLight dirLight;

void main()
{
	vec4 ambientColor = vec4(ambientLight.color * ambientLight.intensity, 1.0f);
	
	float diffuseFactor = dot(normalize(fragNormal), -dirLight.direction);
	vec4 diffuseColor = (diffuseFactor > 0.0f)
						? vec4(ambientLight.color * dirLight.diffuseIntensity * diffuseFactor, 0.0f)
						: vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	outColor = texture2D(diffuseTexture, fragUV) * 
		(ambientColor + diffuseColor);
}
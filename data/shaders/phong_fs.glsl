#version 400 core
struct Material
{
	// TODO: Color
	float specularIntensity;
	float specularPower;
};

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

in vec3 vertexWorldPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform vec3 cameraPosition;
uniform Material material;
uniform AmbientLight ambientLight;
uniform DirectionalLight dirLight;

void main()
{
	vec4 ambientColor = vec4(ambientLight.color * ambientLight.intensity, 1.0f);
	
	vec3 normal = normalize(vertexNormal);
	float diffuseFactor = dot(normal, -dirLight.direction);
	vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if (diffuseFactor > 0.0f)
	{
		diffuseColor = vec4(ambientLight.color * dirLight.diffuseIntensity * diffuseFactor, 1.0f);
		vec3 vertexToEye = normalize(cameraPosition - vertexWorldPosition);
		vec3 lightReflect = normalize(reflect(dirLight.direction, normal));
		
		float specularFactor = dot(vertexToEye, lightReflect);
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.specularPower);
			specularColor = vec4(ambientLight.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	outColor = texture2D(diffuseTexture, vertexUV) * 
		(ambientColor + diffuseColor + specularColor);
}
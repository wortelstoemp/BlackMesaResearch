#version 400 core
struct Material
{
	vec3 specular;    
	float shine;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 vertexWorldPosition;  
in vec3 vertexNormal;
in vec2 vertexUV;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform vec3 cameraPosition;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture2D(diffuseTexture, vertexUV));
  	
    // Diffuse 
    vec3 normal = normalize(vertexNormal);
    vec3 lightDirection = normalize(light.position - vertexWorldPosition);
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture2D(diffuseTexture, vertexUV));
    
    // Specular
    vec3 cameraDirection = normalize(cameraPosition - vertexWorldPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);  
    float specularFactor = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shine);
    vec3 specular = light.specular * (specularFactor * material.specular);  
        
    fragColor =  vec4(ambient + diffuse + specular, 1.0f);
} 
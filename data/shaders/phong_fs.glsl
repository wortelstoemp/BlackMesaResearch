#version 400 core
struct Material
{
	vec3 specular;    
	float shine;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 vertexWorldPosition;  
in vec3 vertexNormal;
in vec2 vertexUV;

out vec4 fragColor;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform vec3 cameraPosition;
uniform Material material;
uniform DirectionalLight dirLight;

void main()
{
    vec3 textureDiffuseResult = vec3(mix(texture(textureDiffuse1, vertexUV), texture(textureDiffuse2, vertexUV), 0.3));
    // Ambient
    vec3 ambient = dirLight.ambient * textureDiffuseResult;
  	
    // Diffuse 
    vec3 normal = normalize(vertexNormal);
    vec3 dirLightDirection = normalize(-dirLight.direction);
    float diffuseFactor = max(dot(normal, dirLightDirection), 0.0);
    vec3 diffuse = dirLight.diffuse * diffuseFactor * textureDiffuseResult;
    
    // Specular
    vec3 cameraDirection = normalize(cameraPosition - vertexWorldPosition);
    vec3 reflectDirection = reflect(-dirLightDirection, normal);  
    float specularFactor = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shine);
    vec3 specular = dirLight.specular * (specularFactor * material.specular);  
        
    fragColor =  vec4(ambient + diffuse + specular, 1.0f);
} 
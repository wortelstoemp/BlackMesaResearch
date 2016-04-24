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
    // TODO(Tom): Fix to no lighting on texture alpha of 0
    vec4 textureDiffuseResult = mix(texture(textureDiffuse1, vertexUV), texture(textureDiffuse2, vertexUV), 0.3);
    // Ambient
    vec4 ambient = vec4(dirLight.ambient, 1.0) * textureDiffuseResult;
  	
    // Diffuse 
    vec3 normal = normalize(vertexNormal);
    vec3 dirLightDirection = normalize(-dirLight.direction);
    float diffuseFactor = max(dot(normal, dirLightDirection), 0.0);
    vec4 diffuse = vec4(dirLight.diffuse, 1.0) * diffuseFactor * textureDiffuseResult;
    
    // Specular
    vec3 cameraDirection = normalize(cameraPosition - vertexWorldPosition);
    vec3 reflectDirection = reflect(-dirLightDirection, normal);  
    float specularFactor = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shine);
    vec4 specular = vec4(dirLight.specular, 1.0) * (specularFactor * vec4(material.specular, 1.0));  
        
    fragColor =  ambient + diffuse + specular;
} 
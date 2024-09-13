#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct DirLight {
    vec3 direction;
	
    vec3 color;
};

struct PointLight {    
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};  
#define MAX_POINT_LIGHT_COUNT 10

// these are being set from shader class
uniform vec3 viewPos;
uniform vec3 skyLight;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform int pointLightCount;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(1, 0, 1); // set default to magenta

    // ambient ligth
	result = skyLight * vec3(texture(material.diffuse, TexCoords));;

	result += CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < pointLightCount; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }    
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoords));
		
	return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 diffuse  = light.color  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoords));

    diffuse  *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
} 
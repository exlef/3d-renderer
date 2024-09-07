#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	bool useColors;
	vec3 diffuseColor;
	vec3 specularColor;
    float shininess;
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// these are being set from shader class
uniform vec3 viewPos; // todo
uniform DirLight dirLight;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(1, 0, 1); // set default to magenta

	result = CalcDirLight(dirLight, norm, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	if(material.useColors)
	{
		ambient = light.ambient * material.diffuseColor;
    	diffuse = light.diffuse * diff * material.diffuseColor;
    	specular = light.specular * spec * material.specularColor;
	}
    return (ambient + diffuse + specular);
	// return diffuse;
}
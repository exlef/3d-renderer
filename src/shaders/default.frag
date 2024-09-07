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

// these are being set from shader class
uniform vec3 viewPos;
uniform vec3 skyLight;
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

	// ambient ligth
	result += skyLight * vec3(texture(material.diffuse, TexCoords));;

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
    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoords));
		
	return (diffuse + specular);
}
#version 430 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
	sampler2D specular;
    float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
};
  
uniform vec3 viewPos;
uniform Material material;
uniform Light pointLights[1];

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	vec3 ambient = pointLights[0].ambient * texture(material.diffuse, TexCoords).rgb;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pointLights[0].position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointLights[0].diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pointLights[0].specular * spec * texture(material.specular, TexCoords).rgb;
	
	float distance = length(pointLights[0].position - FragPos);
	float attenuation = 1.0 / (pointLights[0].constant + pointLights[0].linear * distance + pointLights[0].quadratic * (distance * distance));    
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	FragColor = vec4((ambient + diffuse + specular), 1.0);
}
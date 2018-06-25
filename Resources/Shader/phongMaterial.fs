#version 430 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	vec3 ambient = light.ambient * material.ambient * lightColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 diffuse = light.diffuse * lightColor * (max(dot(norm, lightDir), 0) * material.diffuse);
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 specular = light.specular * lightColor * (pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * material.specular);
	
	FragColor = vec4((ambient + diffuse + specular) * objectColor, 1);
}
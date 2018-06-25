#version 430 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	float ambintStrength = 0.2;
	vec3 ambient = ambintStrength * lightColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 diffuse = max(dot(norm, lightDir), 0) * lightColor;
	
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), 32) * specularStrength * lightColor;
	
	
	FragColor = vec4((ambient + diffuse + specular) * objectColor, 1);
}
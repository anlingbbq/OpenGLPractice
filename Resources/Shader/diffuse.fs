#version 430 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	float ambintStrength = 0.2;
	vec3 ambient = ambintStrength * lightColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 diffuse = max(dot(norm, lightDir), 0) * lightColor;
	
	FragColor = vec4((ambient + diffuse) * objectColor, 1);
}
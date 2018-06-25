#version 430 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	float ambintStrength = 0.2;
	vec3 ambient = ambintStrength * lightColor;
	
	FragColor = vec4(ambient * objectColor, 1);
}
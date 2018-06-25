#version 430 core
out vec4 FragColor;
  
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D ourTexture;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	vec3 color = texture(ourTexture, TexCoords).rgb;
	
	vec3 ambient = 0.05 * color;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 diffuse = max(dot(norm, lightDir), 0) * color;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	// vec3 reflectDir = reflect(-lightDir, norm);
	// vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), 1) * vec3(0.3);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 specular = pow(max(dot(norm, halfwayDir), 0.0), 1) * vec3(0.3);
	
	
	FragColor = vec4(ambient + diffuse + specular, 1);
}
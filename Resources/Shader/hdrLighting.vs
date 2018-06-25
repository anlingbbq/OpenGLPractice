#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoords;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec3 TexCoords;
} vs_out;

uinform mat4 projection;
uinform view;
uinform model;

uniform bool inverse_normals;

void main()
{
	vs_out.FragPos = vec3(model * vec4(pos, 1.0));
	vs_out.TexCoords = texCoords;
	
	vec3 n = inverse_normals ? -normal : normal;
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.Normal = normalize(normalMatrix * n);
	
	gl_Position = projection * view * model * vec4(pos, 1.0);
}
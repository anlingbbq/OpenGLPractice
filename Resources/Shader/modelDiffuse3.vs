#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in float random;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	vec2 texCoords;
	float random;
} vs_out;

void main()
{
	vs_out.texCoords = texCoords;
	vs_out.random = random;
	gl_Position = vec4(pos, 1.0);
} 
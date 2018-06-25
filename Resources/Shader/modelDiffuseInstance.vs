#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * instanceMatrix * vec4(pos, 1.0);
	TexCoords = texCoords;
} 
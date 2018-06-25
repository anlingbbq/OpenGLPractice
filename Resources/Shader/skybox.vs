#version 430 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = pos;
	vec4 rpos = projection * view * vec4(pos, 1.0);
    gl_Position = rpos.xyww;
}  
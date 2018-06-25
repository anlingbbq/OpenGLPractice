#version 430 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 offset;

out vec3 fColor;

void main()
{
	vec2 pos = pos * (gl_InstanceID / 100.0);
    gl_Position = vec4(pos + offset, 0.0, 1.0);
    fColor = color;
}  
#version 430 core
out vec4 fragColor;

uniform vec4 ourColor;

void main()
{
	fragColor = ourColor;
}
#version 430 core
layout (location = 0) out vec4 fragColor;

uniform vec3 lightColor;

void main()
{           
    fragColor = vec4(lightColor, 1.0);
}

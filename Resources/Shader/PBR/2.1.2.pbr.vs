#version 430 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = inTexCoords;
    WorldPos = vec3(model * vec4(Pos, 1.0));
    Normal = mat3(model) * inNormal;   

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
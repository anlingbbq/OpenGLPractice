#version 430 core
layout (location = 0) in vec3 Pos;

uniform mat4 projection;
uniform mat4 view;

out vec3 WorldPos;

void main()
{
    WorldPos = Pos;

    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
    vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);

    gl_Position = clipPos.xyww;
}
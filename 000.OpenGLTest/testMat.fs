#version 430 core
out vec4 FragColor;
uniform vec3 objectColor;
uniform mat3 testMat;

void main()
{
    FragColor = vec4(testMat[0][0], testMat[0][1], testMat[0][1], 1);
}
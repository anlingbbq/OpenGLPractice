#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in VS_OUT {
    vec2 texCoords;
	float random;
} gs_in[];

out vec2 TexCoords; 

uniform float time;

vec4 Decompose(vec4 position, float delay)
{
	if (time > delay)
	{
		float magnitude = 0.5;
		vec3 direction =  magnitude * vec3(1, 1.0, 0.5) * (time - delay); 
		position += vec4(direction, 0.0);
	}
	position = projection * view * model * position;
	return position;
}

void main() {    
    gl_Position = Decompose(gl_in[0].gl_Position, gs_in[0].random);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = Decompose(gl_in[1].gl_Position, gs_in[0].random);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = Decompose(gl_in[2].gl_Position, gs_in[0].random);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}
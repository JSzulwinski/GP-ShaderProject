#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 view;
uniform mat4 projection;

in vec2 TexCoord[];
in vec3 Normal[];

in VS_OUT 
{
    vec3 normal;
} 
gs_in[];

in vec2 v_tex[];

out vec2 TexCoords; 

out vec2 tex;

const float SCALE = 0.03;

void GeometryNormal(int index)
{
    gl_Position = gl_in[index].gl_Position;
	TexCoords = v_tex[0];
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * SCALE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GeometryNormal(0);
}

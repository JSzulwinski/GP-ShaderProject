#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;

out vec3 v_norm;
out vec2 v_tex;
out vec4 v_pos; 

out VS_OUT 
{
    vec3 normal;
} 
vs_out;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	v_norm = aNormal;
    v_tex = aTexCoord;

    TexCoord = aTexCoord;
    gl_Position = model * vec4(aPos, 1.0);
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));   
	vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * aNormal, 0.0)));

	Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
}


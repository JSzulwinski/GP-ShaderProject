
#version 400

in vec2 tex;

uniform sampler2D water;

out vec4 fragcolor;

void main()
{
	fragcolor = vec4(1.0, 0, 0, 1.0);
}


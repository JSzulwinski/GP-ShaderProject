#version 400
//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;

in vec2 TexCoords;

//Unifrom
uniform sampler2D bricks;

void main()
{
	fragcolor = vec4(texture2D(bricks, TexCoords).rgb, 1.0);
}

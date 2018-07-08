#version 450 core

in vec3 colourFrag;
in vec2 textureCoord;

out vec4 color;

uniform sampler2D udiff;

void main()
{
	vec4 txt = texture2D(udiff, textureCoord);
	color = vec4(colourFrag, txt.w);
}
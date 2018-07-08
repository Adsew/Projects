#version 450 core

in vec2 textureCoord;

out vec4 color;

uniform sampler2D udiff;

void main()
{
	color = texture2D(udiff, textureCoord);
	//color = vec4(1, 0, 0, 1);
}
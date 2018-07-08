#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec2 texture;

out vec3 colourFrag;
out vec2 textureCoord;
 
void main()
{
	colourFrag = colour;
	textureCoord = texture;
	gl_Position = vec4(position, 1);
}
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;

out vec4 colourFrag;
 
void main()
{
	colourFrag = colour;
	gl_Position = vec4(position, 1);
}
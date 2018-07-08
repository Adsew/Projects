#version 450 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture;

out vec2 textureCoord;
 
void main()
{
	textureCoord = texture;
	gl_Position = vec4(position, 0, 1);
}
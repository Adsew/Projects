#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 world_matrix;
 
void main()
{
	gl_Position = world_matrix * vec4(position, 1.0);
}
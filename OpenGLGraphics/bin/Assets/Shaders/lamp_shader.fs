#version 450 core

uniform vec3 lamp_color = vec3(1.0, 1.0, 1.0);
out vec4 color;

void main()
{
	color = vec4(lamp_color, 1.0);
}
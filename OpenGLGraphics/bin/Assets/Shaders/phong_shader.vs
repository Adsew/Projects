#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

uniform mat4 model_matrix;
uniform mat4 world_matrix;


out Data
{
	// shading
	vec3 surface_position;
	vec3 normal;
	// texture
	vec2 textCoord;
} vs_data;
 
 
void main()
{
	vs_data.textCoord = textureCoord;										// pass textCoord
	vs_data.surface_position = vec3(model_matrix * vec4(position, 1.0));	// convert position into model space
	vs_data.normal = mat3(transpose(inverse(model_matrix))) * normal;		// convert normal to account for non-uniform scaling
	gl_Position = world_matrix * vec4(position, 1.0);
}
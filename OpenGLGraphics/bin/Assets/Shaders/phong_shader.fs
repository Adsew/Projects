#version 450 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float intensity;
	float specular_strength;
};

uniform sampler2D udiff;

uniform Light lights[3];

in Data
{
	// shading
	vec3 surface_position;
	vec3 normal;
	vec3 eye;
	mat4 view_matrix;
	// texture
	vec2 textCoord;
} fs_data;

out vec4 color;

void main()
{
	vec4 object_color = texture2D(udiff, fs_data.textCoord);
	
	vec3 final_color = vec3(0.0);
	for (int i = 0; i < lights.length(); i++)
	{
		vec3 light_position = vec3(fs_data.view_matrix * vec4(lights[i].position, 1.0));
		vec3 norm = normalize(fs_data.normal);

		//*****************************************
		//	AMBIENT
		//
		vec3 ambient = lights[i].intensity * lights[i].ambient;

		//******************************************
		//	DIFFUSE
		//
		
		vec3 lightDir = normalize(light_position - fs_data.surface_position);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lights[i].ambient;
		
		//*******************************************
		//	SPECULAR
		//
		
		vec3 viewDir = normalize(fs_data.eye - fs_data.surface_position);
		vec3 reflectDir = reflect(-lightDir, norm);
		
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = lights[i].specular_strength * spec * lights[i].ambient;
		
		final_color = final_color + ((ambient + diffuse + specular) * object_color.rgb);
	}
	color = vec4(final_color, 1.0);
	//color = texture2D(udiff, fs_data.textCoord);
}
#version 450 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
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

vec3 calcPointLighting(Light light, vec3 normal, vec3 viewDirection)
{
	vec3 light_position = vec3(fs_data.view_matrix * vec4(light.position, 1.0));
		
	//*****************************************
	//	ATTENUATION
	//
	float distance = length(light_position - fs_data.surface_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance * light.quadratic * (distance * distance));

	//*****************************************
	//	AMBIENT
	//
	vec3 ambient = light.ambient * attenuation;

	//******************************************
	//	DIFFUSE
	//
	
	vec3 lightDir = normalize(light_position - fs_data.surface_position);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * attenuation;
	
	//*******************************************
	//	SPECULAR
	//
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfDir = normalize(lightDir + viewDirection);
	
	float spec = pow(max(dot(normal, halfDir), 0.0), 32);
	vec3 specular = spec * light.specular * attenuation;

	return (ambient + diffuse + specular);
}



void main()
{
	vec4 object_color = texture2D(udiff, fs_data.textCoord);
	
	vec3 final_color = vec3(0.0);
	
	vec3 viewDir = normalize(fs_data.eye - fs_data.surface_position);
	vec3 norm = normalize(fs_data.normal);
	
	for (int i = 0; i < lights.length(); i++)
	{	
		final_color += calcPointLighting(lights[i], norm, viewDir) * vec3(object_color);
	}
	
	color = vec4(final_color, 1.0);
}
#version 450 core

uniform struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} directional_light;

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float outerCutoff;
	float constant;
	float linear;
	float quadratic;
	vec3 direction;
	float cutoff;
	bool on;
};

uniform PointLight lights[3];

uniform SpotLight flashlight;

uniform struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shineness;
} material;

uniform vec3 eye;

uniform bool useTexture;
uniform vec4 meshDiffuse;
uniform vec4 meshSpec;

in Data
{
	// shading
	vec3 surface_position;
	vec3 normal;
	//mat4 view_matrix;
	// texture
	vec2 textCoord;
} fs_data;

out vec4 color;

// FORWARD DECLARE FUNCTIONS
vec3 calcPointLighting(PointLight light, vec3 normal, vec3 viewDirection, vec4 difColor, vec4 specColor);
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec4 difColor, vec4 specColor);
vec3 calcSpotLighting(SpotLight light, vec3 normal, vec3 viewDirection, vec4 difColor, vec4 specColor);

void main()
{
	vec3 norm = normalize(fs_data.normal);
	vec3 viewDir = normalize(eye - fs_data.surface_position);
	
	vec4 diffuseColor;
	vec4 specularColor;
	
	if (useTexture)
	{
		diffuseColor = texture(material.diffuse, fs_data.textCoord);
		specularColor = texture(material.specular, fs_data.textCoord);
	} else 
	{
		diffuseColor = meshDiffuse;
		specularColor = meshSpec;
	}
	
	vec3 final_color = calcDirectionalLight(directional_light, norm, viewDir, diffuseColor, specularColor);
	
	for (int i = 0; i < lights.length(); i++)
	{	
		final_color += calcPointLighting(lights[i], norm, viewDir, diffuseColor, specularColor);
	}
	final_color += calcSpotLighting(flashlight, norm, viewDir, diffuseColor, specularColor);
	
	color = vec4(final_color, 1.0);
}

// calculate the point light. this is calculated for every light
vec3 calcPointLighting(PointLight light, vec3 normal, vec3 viewDirection, vec4 difColor, vec4 specColor)
{
	//*****************************************
	//	ATTENUATION
	//	-- light intensity lowers over time
	float distance = length(light.position - fs_data.surface_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance * light.quadratic * (distance * distance));

	//*****************************************
	//	AMBIENT
	//
	vec3 ambient = light.ambient * difColor.rgb * attenuation;

	//******************************************
	//	DIFFUSE
	//
	// calculate lght direction
	vec3 lightDir = normalize(light.position - fs_data.surface_position);
	// get the cose theta between the of the vertex and the light direction from the surface
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * difColor.rgb * attenuation;
	
	//*******************************************
	//	SPECULAR
	//
	// get the reflection from the light direction and the normal
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfDir = normalize(lightDir + viewDirection);
	
	float spec = pow(max(dot(normal, halfDir), 0.0), material.shineness);
	vec3 specular = spec * light.specular * specColor.rgb * attenuation;

	return (ambient + diffuse + specular);
}

// calculate the directional light
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec4 difColor, vec4 specColor)
{
	vec3 directional_l_dir = normalize(-directional_light.direction);
	
	// ambient
	vec3 ambient = light.ambient * difColor.rgb;
	
	// diffuse
	float diff = max(dot(normal, directional_l_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * difColor.rgb;
	
	// specular
	vec3 reflectDir = reflect(-directional_l_dir, normal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shineness);
	vec3 specular = light.specular * spec * specColor.rgb;
	
	return ambient + diffuse + specular;
}

vec3 calcSpotLighting(SpotLight light, vec3 normal, vec3 viewDirection, vec4 difColor, vec4 specColor)
{
	
	if (light.on)
	{
		vec3 lightDir = normalize(light.position - fs_data.surface_position);
		float theta = dot(lightDir, normalize(-light.direction));
		
		//*****************************************
		//  SPOT TAPERING
		//
		float epsilon   = light.cutoff - light.outerCutoff;
		float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);  
		
		//*****************************************
		//	ATTENUATION
		//	-- light intensity lowers over time
		float distance = length(light.position - fs_data.surface_position);
		float attenuation = 1.0 / (light.constant + light.linear * distance * light.quadratic * (distance * distance));

		//*****************************************
		//	AMBIENT
		//
		vec3 ambient = light.ambient * difColor.rgb * attenuation;

		//******************************************
		//	DIFFUSE
		//
		// get the cose theta between the of the vertex and the light direction from the surface
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = diff * light.diffuse * difColor.rgb * attenuation * intensity;
	
		//*******************************************
		//	SPECULAR
		//
		// get the reflection from the light direction and the normal
		vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfDir = normalize(lightDir + viewDirection);
	
		float spec = pow(max(dot(normal, halfDir), 0.0), material.shineness);
		vec3 specular = spec * light.specular * specColor.rgb * attenuation * intensity;
		
		return (ambient + diffuse + specular);
	}
	
	return vec3(0,0,0);
}
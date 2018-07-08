const int MAX_LIGHTS = 3;

struct LighSource
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	float intensity;
};

// block of data holding max 3 light sources
layout (std140, binding = 3) uniform LightBlock
{
	LighSource lights[MAX_LIGHTS];
};
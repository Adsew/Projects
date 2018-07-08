#pragma once

#include "ShaderProgram.h"
#include "IRenderable.h"

// render a simple object with 1 color. used for rendering lights and other placeholders that are not
// normally rendered
class LampRenderer : public IRenderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(LampRenderer, IRenderable);
protected:
	struct Vertex
	{
		glm::vec3 position;
	};

	glm::vec3 m_color;
	std::vector<Vertex> m_vertices;
	unsigned int m_vao;
	unsigned int m_vbo;
	std::unique_ptr<ShaderProgram> m_shaderProgram;
	void loadMesh(const char* path);
	bool m_configured;
public:
	LampRenderer();
	~LampRenderer();
	void load(tinyxml2::XMLElement* element);
	void initialize() override;
	void render() override;
};


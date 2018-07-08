#pragma once

#include "TextureAsset.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "IRenderable.h"

struct MeshVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
};

enum MeshFaceType
{
	kTRIANGLE = 0,
	kQUADS = 1
};

// complex render that renders meshes
class MeshRenderer : public IRenderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(MeshRenderer, IRenderable);
protected:
	bool m_useTexture;
	glm::vec4 m_diffuse;
	glm::vec4 m_specular;

	std::vector<MeshVertex> m_vertices;
	unsigned int m_vao;
	unsigned int m_vbo;
	bool m_configured;
	Material m_material;
	std::unique_ptr<ShaderProgram> m_shaderProgram;
	void loadMesh(const char* path);
	void update(float _deltaTime) override;

	MeshFaceType m_faceType;

public:
	MeshRenderer();
	virtual ~MeshRenderer();
	void load(tinyxml2::XMLElement* element);
	virtual void initializeOpenGL();
	void render() override;
	inline void addVertex(const MeshVertex& vertex)
	{
		m_vertices.push_back(vertex);
	}
	void updateVertices(const glm::vec3& v);
};
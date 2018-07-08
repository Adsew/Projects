#include "Core.h"
#include "MeshRenderer.h"

#include <fstream>
#include <sstream>

#include "CameraComponent.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"

IMPLEMENT_DYNAMIC_CLASS(MeshRenderer);

void MeshRenderer::loadMesh(const char * path)
{
	std::vector<glm::vec3> t_v;	// position
	std::vector<glm::vec3> t_n;  // normal
	std::vector<glm::vec2> t_t; // texture;

	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		glm::vec3 v_min;
		glm::vec3 v_max;
		bool firstPosition = true;
		for (std::string line; std::getline(file, line); )
		{
			// do not process empty lines...
			if (line.empty() || line[0] == '#')
			{
				continue;
			}

			if (line.substr(0, 2) == "v ")      // vertex position
			{
				std::istringstream ss(line.substr(2));
				glm::vec3 p;
				ss >> p.x; ss >> p.y; ss >> p.z;
				t_v.push_back(p);

				if (firstPosition)
				{
					firstPosition = false;
					v_min = p;
					v_max = p;
				}
				else {
					// x
					if (p.x < v_min.x)
					{
						v_min.x = p.x;
					}
					else if (p.x > v_max.x)
					{
						v_max.x = p.x;
					}
					// y
					if (p.y < v_min.y)
					{
						v_min.y = p.y;
					}
					else if (p.y > v_max.y)
					{
						v_max.y = p.y;
					}
					// z
					if (p.z < v_min.z)
					{
						v_min.z = p.z;
					}
					else if (p.z > v_max.z)
					{
						v_max.z = p.z;
					}
				}
			}
			else if (line[0] == 'f')           // vertex face
			{
				unsigned long a, b, c;
				unsigned long na, nb, nc;
				unsigned long ta, tb, tc;

				std::istringstream s(line.substr(2));
				std::string data;

				// vertex 1
				std::getline(s, data, '/');
				a = std::stoul(data);
				std::getline(s, data, '/');
				ta = std::stoul(data);
				std::getline(s, data, ' ');
				na = std::stoul(data);

				// vertex 2
				std::getline(s, data, '/');
				b = std::stoul(data);
				std::getline(s, data, '/');
				tb = std::stoul(data);
				std::getline(s, data, ' ');
				nb = std::stoul(data);

				// vertex 3
				std::getline(s, data, '/');
				c = std::stoul(data);
				std::getline(s, data, '/');
				tc = std::stoul(data);
				std::getline(s, data, ' ');
				nc = std::stoul(data);

				// c++ is zero based
				--a; --b; --c;
				--na; --nb; --nc;
				--ta; --tb; --tc;

				MeshVertex v1;
				MeshVertex v2;
				MeshVertex v3;

				v1.position = t_v[a];
				v1.normal = t_n[na];
				v1.texture = t_t[ta];

				v2.position = t_v[b];
				v2.normal = t_n[nb];
				v2.texture = t_t[tb];

				v3.position = t_v[c];
				v3.normal = t_n[nc];
				v3.texture = t_t[tc];

				m_vertices.push_back(v1);
				m_vertices.push_back(v2);
				m_vertices.push_back(v3);
			}
			else if (line.substr(0, 2) == "vn")	 // vertex normal
			{
				std::istringstream ss(line.substr(2));
				glm::vec3 n;
				ss >> n.x; ss >> n.y; ss >> n.z;
				t_n.push_back(n);
			}
			else if (line.substr(0, 2) == "vt")	 // vertex texture coord
			{
				std::istringstream ss(line.substr(2));
				glm::vec2 t;
				ss >> t.x; ss >> t.y;
				t.y = 1 - t.y;	// flip the UVs
				t_t.push_back(t);
			}
			else if (line[0] == 'o')	         // object name
			{
				setName(line.substr(2));
			}
		}
		if (auto transform = getGameObject()->transform())
		{
			glm::vec3 v = v_max - v_min;
			v.x /= 2;
			v.y /= 2;
			v.z /= 2;
			v = v_max - v;
			updateVertices(v);
		}
	}
	else {
		std::cout << "MeshRenderer was unable to open the OBJ file" << std::endl;
	}

	file.close();
}


MeshRenderer::MeshRenderer()
	: m_vao(0),
	m_vbo(0),
	m_configured(false),
	m_shaderProgram(nullptr),
	m_faceType(MeshFaceType::kTRIANGLE),
	m_useTexture(true)
{
}

MeshRenderer::~MeshRenderer()
{
	if (m_configured)
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}
}

void MeshRenderer::load(tinyxml2::XMLElement * element)
{
	Component::load(element);

	if (element->Attribute("type", "QUADS"))
	{
		m_faceType = MeshFaceType::kQUADS;
	}
	else {
		m_faceType = MeshFaceType::kTRIANGLE;
	}

	// load mesh data
	if (auto path = element->FirstChildElement("path"))
	{
		loadMesh(path->GetText());
		initializeOpenGL();
	}

	XMLElement* mat = element->FirstChildElement(TO_STRING(Material));
	if (mat != nullptr)
	{
		m_material.load(mat);
	}

	XMLElement* shaderProgram = element->FirstChildElement(TO_STRING(ShaderProgram));
	if (shaderProgram != nullptr)
	{
		m_shaderProgram = std::make_unique<ShaderProgram>();
		m_shaderProgram->load(shaderProgram);
		m_shaderProgram->initialize();
	}

	XMLElement* xmlDiffuse = element->FirstChildElement("diffuse");
	if (xmlDiffuse != nullptr)
	{
		m_useTexture = false;
		xmlDiffuse->QueryFloatAttribute("r", &m_diffuse.r);
		xmlDiffuse->QueryFloatAttribute("g", &m_diffuse.g);
		xmlDiffuse->QueryFloatAttribute("b", &m_diffuse.b);
		if (xmlDiffuse->QueryFloatAttribute("a", &m_diffuse.a) == XML_NO_ATTRIBUTE)
		{
			m_diffuse.a = 1.0f;
		}
	}
	XMLElement* xmlSpec = element->FirstChildElement("specular");
	if (xmlSpec != nullptr)
	{
		m_useTexture = false;
		xmlSpec->QueryFloatAttribute("r", &m_specular.r);
		xmlSpec->QueryFloatAttribute("g", &m_specular.g);
		xmlSpec->QueryFloatAttribute("b", &m_specular.b);
		if (xmlSpec->QueryFloatAttribute("a", &m_specular.a) == XML_NO_ATTRIBUTE)
		{
			m_specular.a = 1.0f;
		}
	}
}

void MeshRenderer::initializeOpenGL()
{
	// generate a vao and vbo
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// start using the vao
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(MeshVertex), &m_vertices[0], GL_STATIC_DRAW);
	// VERTEX POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), nullptr);

	// VERTEX NORMAL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, normal));

	// VERTEX TEXTURE
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, texture));

	// do not allow other objects to edit mesh
	glBindVertexArray(0);

	m_configured = true;

	RenderManager::instance().attachRenderable(std::static_pointer_cast<IRenderable>(shared_from_this()));
}


void MeshRenderer::update(float _deltaTime)
{
	if (m_shaderProgram == nullptr)
	{
		return;
	}


}

void MeshRenderer::render()
{
	if (!m_configured)
	{
		return;
	}

	if (auto go = m_gameObject.lock())
	{

		auto transorm = go->transform();
		if (transorm == nullptr)
		{
			LOG("MeshRenderer found nullptr transform");
			return;
		}

		if (m_shaderProgram == nullptr)
		{
			LOG("MeshRenderer found nullptr ShaderProgram");
			return;
		}

		auto camera = GameObjectManager::instance().getCamera();
		if (camera == nullptr)
		{
			LOG("MeshRenderer could not find camera");
			return;
		}

		auto cc = std::static_pointer_cast<CameraComponent>(camera->getComponent<CameraComponent>());
		if (cc == nullptr)
		{
			LOG("MeshRenderer could not find camera component");
			return;
		}

		m_shaderProgram->use();
		m_shaderProgram->bindUniform<const glm::vec3&>("eye", camera->transform()->position());

		m_shaderProgram->bindUniform<const glm::mat4&>("model_matrix", transorm->getModelMatrix());
		//m_shaderProgram->bindUniform<const glm::mat4&>("view_matrix", cc->getViewMatrix());
		m_shaderProgram->bindUniform<const glm::mat4&>("world_matrix", transorm->getWorldMatrix());

		RenderManager::instance().bindLights(m_shaderProgram.get());

		m_material.use();

		m_shaderProgram->bindUniform<bool>("useTexture", m_useTexture);
		if (!m_useTexture)
		{
			m_shaderProgram->bindUniform<const glm::vec4&>("meshDiffuse", m_diffuse);
			m_shaderProgram->bindUniform<const glm::vec4&>("meshSpec", m_specular);
		}
		else {
			m_shaderProgram->bindUniform<int>("material.diffuse", m_material.getDiffuseUnit());
			m_shaderProgram->bindUniform<int>("material.specular", m_material.getSpecularUnit());
		}

		m_shaderProgram->bindUniform<float>("material.shineness", m_material.getShineness());

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
		glBindVertexArray(0);

		m_material.disable();

		m_shaderProgram->disable();
	}


}

void MeshRenderer::updateVertices(const glm::vec3 & v)
{
	for (auto& vert : m_vertices)
	{
		vert.position = vert.position - v;
	}
}

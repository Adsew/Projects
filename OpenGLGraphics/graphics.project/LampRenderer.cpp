#include "Core.h"
#include "LampRenderer.h"

#include <fstream>
#include <sstream>

#include "RenderManager.h"
#include "GameObject.h"

IMPLEMENT_DYNAMIC_CLASS(LampRenderer);

void LampRenderer::loadMesh(const char * path)
{
	std::vector<glm::vec3> t_v;	// position
	std::vector<glm::vec3> t_n;  // normal
	std::vector<glm::vec2> t_t; // texture;

	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		bool flipUV = false;
		for (std::string line; std::getline(file, line); )
		{
			// do not process empty lines...
			if (line.empty())
			{
				continue;
			}
			else if (line[0] == '#')
			{
				std::istringstream s(line.substr(2));
				std::string data;
				std::getline(s, data, ' ');
				if (data == "Blender")
				{
					std::cout << "OBJ file from Blender" << std::endl;
					flipUV = true;
				}
			}

			if (line.substr(0, 2) == "v ")      // vertex position
			{
				std::istringstream ss(line.substr(2));
				glm::vec3 p;
				ss >> p.x; ss >> p.y; ss >> p.z;
				t_v.push_back(p);
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

				Vertex v1;
				Vertex v2;
				Vertex v3;

				v1.position = t_v[a];

				v2.position = t_v[b];

				v3.position = t_v[c];

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
				if (flipUV)
				{
					t.y = 1 - t.y;	// flip the UVs
				}
				t_t.push_back(t);
			}
			else if (line[0] == 'o')	         // object name
			{
				setName(line.substr(2));
			}
		}
	}
	else {
		std::cout << "MeshRenderer was unable to open the OBJ file" << std::endl;
	}

	file.close();
}

LampRenderer::LampRenderer()
	: m_configured(false)
{
}


LampRenderer::~LampRenderer()
{
}

void LampRenderer::load(tinyxml2::XMLElement * element)
{
	Component::load(element);

	if (auto path = element->FirstChildElement("path"))
	{
		loadMesh(path->GetText());
	}

	XMLElement* shaderProgram = element->FirstChildElement(TO_STRING(ShaderProgram));
	if (shaderProgram != nullptr)
	{
		m_shaderProgram = std::make_unique<ShaderProgram>();
		m_shaderProgram->load(shaderProgram);
		m_shaderProgram->initialize();
	}

	XMLElement* color = element->FirstChildElement("color");
	if (color != nullptr)
	{
		XMLElement* data = color->FirstChildElement("x");
		if (data != nullptr)
		{
			data->QueryFloatText(&m_color.x);
		}
		data = color->FirstChildElement("y");
		if (data != nullptr)
		{
			data->QueryFloatText(&m_color.y);
		}
		data = color->FirstChildElement("z");
		if (data != nullptr)
		{
			data->QueryFloatText(&m_color.z);
		}
	}
}

void LampRenderer::initialize()
{
	Component::initialize();

	// generate a vao and vbo
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// start using the vao
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	// VERTEX POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	// do not allow other objects to edit mesh
	glBindVertexArray(0);

	m_configured = true;

	RenderManager::instance().attachRenderable(std::static_pointer_cast<IRenderable>(shared_from_this()));
}

void LampRenderer::render()
{
	if (!m_configured)
	{
		return;
	}

	if (m_shaderProgram == nullptr)
	{
		LOG("SimpleRenderer found nullptr ShaderProgram");
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

		m_shaderProgram->use();
		m_shaderProgram->bindUniform<const glm::vec3&>("lamp_color", m_color);
		m_shaderProgram->bindUniform<const glm::mat4&>("world_matrix", transorm->getWorldMatrix());

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
		glBindVertexArray(0);
		m_shaderProgram->disable();
	}
}

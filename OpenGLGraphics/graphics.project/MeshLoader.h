#pragma once

#include <fstream>

#include "MeshRenderer.h"
#include "Collider.h"

#include <sstream>

#include "GameObjectManager.h"

namespace loader
{
	static void loadOBJ(const char* path)
	{
		std::vector<glm::vec3> t_v;	// position
		std::vector<glm::vec3> t_n;  // normal
		std::vector<glm::vec2> t_t; // texture;

		std::ifstream file;
		file.open(path);

		std::shared_ptr<MeshRenderer> renderer;
		glm::vec3 v_min;
		glm::vec3 v_max;

		if (file.is_open())
		{
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
					unsigned long a = 0, b = 0, c = 0;
					unsigned long na = 0, nb = 0, nc = 0;
					unsigned long ta = 0, tb = 0, tc = 0;

					std::istringstream s(line.substr(2));
					std::string data;

					// vertex 1
					std::getline(s, data, '/');
					if (!data.empty())
					{
						a = std::stoul(data);
					}
					std::getline(s, data, '/');
					if (!data.empty())
					{
						ta = std::stoul(data);
					}
					std::getline(s, data, ' ');
					if (!data.empty())
					{
						na = std::stoul(data);
					}

					// vertex 2
					std::getline(s, data, '/');
					if (!data.empty())
					{
						b = std::stoul(data);
					}
					std::getline(s, data, '/');
					if (!data.empty())
					{
						tb = std::stoul(data);
					}
					std::getline(s, data, ' ');
					if (!data.empty())
					{
						nb = std::stoul(data);
					}

					// vertex 3
					std::getline(s, data, '/');
					if (!data.empty())
					{
						c = std::stoul(data);
					}
					std::getline(s, data, '/');
					if (!data.empty())
					{
						tc = std::stoul(data);
					}
					std::getline(s, data, ' ');
					if (!data.empty())
					{
						nc = std::stoul(data);
					}

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

					if (renderer != nullptr)
					{
						renderer->addVertex(v1);
						renderer->addVertex(v2);
						renderer->addVertex(v3);
					}
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
					if (renderer != nullptr)
					{
						if (auto collider = renderer->getGameObject()->addComponent<Collider>())
						{
							collider->min = v_min;
							collider->max = v_max;

						}

						if (auto transform = renderer->getGameObject()->transform())
						{
							glm::vec3 v = v_max - v_min;
							v.x /= 2;
							v.y /= 2;
							v.z /= 2;
							v = v_max - v;
							transform->setPosition(v);
							renderer->updateVertices(v);
						}

						renderer->initializeOpenGL();
					}
					const auto name = line.substr(2);
					auto go = GameObjectManager::instance().findGameObject(name.c_str());
					if (go != nullptr)
					{
						if (auto component = go->getComponent<MeshRenderer>())
						{
							renderer = component;
						}
					}
					else {
						renderer = nullptr;
					}

					firstPosition = true;
					v_min = glm::vec3();
					v_max = glm::vec3();
				}

			}
			if (renderer != nullptr)
			{
				if (auto collider = renderer->getGameObject()->addComponent<Collider>())
				{
					collider->min = v_min;
					collider->max = v_max;
				}

				if (auto transform = renderer->getGameObject()->transform())
				{
					glm::vec3 v = v_max - v_min;
					v.x /= 2;
					v.y /= 2;
					v.z /= 2;
					v = v_max - v;
					transform->setPosition(v);
					renderer->updateVertices(v);
				}
				renderer->initializeOpenGL();
			}
		}
		else {
			std::cout << "MeshRenderer was unable to open the OBJ file" << std::endl;
		}

		file.close();
	}

}
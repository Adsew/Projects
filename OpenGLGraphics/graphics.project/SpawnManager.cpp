#include "Core.h"
#include "SpawnManager.h"

#include "GameObject.h"
#include "GameObjectManager.h"

IMPLEMENT_DYNAMIC_CLASS(SpawnManager);

void SpawnManager::load(XMLElement * element)
{
	std::list<glm::vec3> m_locations;
	std::list<std::shared_ptr<GameObject>> m_prefabs;

	// load positions
	XMLElement* xmlPositions = element->FirstChildElement("positions");
	if (xmlPositions != nullptr)
	{
		auto xmlPosition = xmlPositions->FirstChildElement();
		while (xmlPosition != nullptr)
		{
			glm::vec3 location;
			xmlPosition->QueryFloatAttribute("x", &location.x);
			xmlPosition->QueryFloatAttribute("y", &location.y);
			xmlPosition->QueryFloatAttribute("z", &location.z);
			m_locations.push_back(location);
			xmlPosition = xmlPosition->NextSiblingElement();
		}
	}

	// load prefabs
	XMLElement* xmlPrefabs = element->FirstChildElement("prefabs");
	if (xmlPrefabs != nullptr)
	{
		XMLElement* xmlPrefab = xmlPrefabs->FirstChildElement();
		while (xmlPrefab != nullptr)
		{
			auto go = std::make_shared<GameObject>();
			go->load(xmlPrefab);
			go->initialize();

			m_prefabs.push_back(go);

			xmlPrefab = xmlPrefab->NextSiblingElement();
		}
	}

	if (m_locations.size() >= m_prefabs.size())
	{
		// change lists to referenced vector
		std::vector<std::reference_wrapper<std::shared_ptr<GameObject>>> pref(m_prefabs.begin(), m_prefabs.end());
		std::vector<std::reference_wrapper<glm::vec3>> locs(m_locations.begin(), m_locations.end());

		// shuffle locations && prefabs
		std::random_shuffle(pref.begin(), pref.end());
		std::random_shuffle(locs.begin(), locs.end());

		// transfer data back to list
		std::list<std::shared_ptr<GameObject>> m_shuffledPrefabs(pref.begin(), pref.end());
		std::list<glm::vec3> m_shuffledLocs(locs.begin(), locs.end());

		for (auto it = m_shuffledPrefabs.begin(); it != m_shuffledPrefabs.end(); /*blank*/)
		{
			// set position of prefab
			(*it)->transform()->setPosition(m_shuffledLocs.front());
			GameObjectManager::instance().addGameObject((*it));
			// remove prefab and location from list
			m_shuffledLocs.erase(m_shuffledLocs.begin());
			m_shuffledPrefabs.erase(it++);
		}
	}
}

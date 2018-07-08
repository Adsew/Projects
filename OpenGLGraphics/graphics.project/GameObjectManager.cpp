#include "Core.h"
#include "GameObjectManager.h"

#include "CameraComponent.h"

#include "MeshLoader.h"

#include "IsPickUpable.h"


GameObjectManager::GameObjectManager()
	: IManager(TO_STRING(GameObjectManager)),
	m_activeCamera(std::shared_ptr<GameObject>())
{
}


GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::initialize()
{
	REGISTER_DYNAMIC_CLASS(IsPickUpable);

	std::string path;
	path.append("Config/").append(m_name).append(".xml");

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(path.c_str());
	if (error != tinyxml2::XML_NO_ERROR)
	{
		doc.PrintError();
	}

	tinyxml2::XMLElement* root = doc.FirstChildElement(m_name.c_str());
	if (root == nullptr)
	{
		std::cout << "GameObjectManager could not find xml information" << std::endl;
		return;
	}

	tinyxml2::XMLElement* objects = root->FirstChildElement("Objects");
	if (objects == nullptr) { return; }

	tinyxml2::XMLElement* object = objects->FirstChildElement();
	while (object != nullptr)
	{
		if (auto strClass = object->Name())
		{
			Object* obj = CRtti::constructObject(strClass);
			if (obj != nullptr && obj->isA(GameObject::getClassHashCode()))
			{
				auto ptr = std::shared_ptr<GameObject>(static_cast<GameObject*>(obj));
				ptr->load(object);
				ptr->initialize();

				m_objects.push_back(ptr);
			}
		}
		object = object->NextSiblingElement();
	}

	tinyxml2::XMLElement* renderers = root->FirstChildElement("Renderers");
	if (renderers != nullptr)
	{
		tinyxml2::XMLElement* renderer = renderers->FirstChildElement("Renderer");
		while (renderer != nullptr)
		{
			if (auto path = renderer->Attribute("path"))
			{
				loader::loadOBJ(path);
			}
			renderer = renderer->NextSiblingElement();
		}
	}
}

void GameObjectManager::update(float _deltaTime)
{
	if (auto camera = m_activeCamera.lock())
	{
		camera->update(_deltaTime);
	}
	for (auto it = m_objects.begin(); it != m_objects.end(); /* blank */)
	{
		if ((*it) == nullptr)
		{
			m_objects.erase(it++);
		}
		else if ((*it)->isMarkedForDelete())
		{
			m_objects.erase(it++);
		}
		else {
			(*it)->update(_deltaTime);
			++it;
		}
	}
}

std::shared_ptr<GameObject> GameObjectManager::getCamera()
{
	return m_activeCamera.lock();
}

void GameObjectManager::setCamera(std::shared_ptr<GameObject> go)
{
	m_activeCamera = go;
}

std::shared_ptr<GameObject> GameObjectManager::findGameObject(const char* name) const
{
	const STRCODE code = getHashCode(name);
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [&, code](const auto& go) {
		return go->getNameHashCode() == code;
	});
	return it != m_objects.end() ? (*it) : nullptr;
}

void GameObjectManager::addGameObject(std::shared_ptr<GameObject> go)
{
	if (go != nullptr)
	{
		m_objects.push_back(go);
	}
}

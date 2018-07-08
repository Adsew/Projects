#include "Core.h"
#include "AssetManager.h"

#include "Asset.h"

#include <algorithm>


AssetManager::AssetManager()
	: IManager(TO_STRING(AssetManager))
{
}


AssetManager::~AssetManager()
{
}

void AssetManager::update(float _deltaTime)
{
}

void AssetManager::initialize()
{
	REGISTER_ABSTRACT_CLASS(Asset);

	std::string path;
	path.append("Config/").append(m_name).append(".xml");

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(path.c_str());

	if (error != tinyxml2::XML_NO_ERROR)
	{
		doc.PrintError();
		return;
	}

	tinyxml2::XMLElement* root = doc.FirstChildElement(m_name.c_str());
	if (root == nullptr)
	{
		std::cout << "could not find AssetManager root element" << std::endl;
	}
	tinyxml2::XMLElement* assets = root->FirstChildElement("Assets");
	if (assets == nullptr)
	{
		std::cout << "cound not find assets in AssetManager" << std::endl;
	}
	tinyxml2::XMLElement* asset = assets->FirstChildElement();
	while (asset != nullptr)
	{
		if (auto strClass = asset->Name())
		{
			Object* object = CRtti::constructObject(strClass);
			if (object != nullptr && object->isA(Asset::getClassHashCode()))
			{
				auto ptr = std::shared_ptr<Asset>(static_cast<Asset*>(object));
				ptr->load(asset);
				ptr->initialize();
				m_assets.push_back(ptr);
			}
		}
		asset = asset->NextSiblingElement();
	}
}

std::shared_ptr<Asset> AssetManager::getAsset(const std::string & name)
{
	auto it = std::find_if(m_assets.cbegin(), m_assets.cend(), [name](auto& asset) {
		return asset->getName() == name;
	});

	if (it == m_assets.end())
	{
		return nullptr;
	}

	return (*it);
}

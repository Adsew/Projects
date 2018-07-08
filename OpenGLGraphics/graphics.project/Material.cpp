#include "Core.h"
#include "Material.h"

#include "TextureAsset.h"

#include "AssetManager.h"

IMPLEMENT_DYNAMIC_CLASS(Material);

Material::Material()
	: m_diffuseTexture(std::shared_ptr<TextureAsset>()),
	m_specularTexture(std::shared_ptr<TextureAsset>()),
	m_shineness(0.0f)
{
}

void Material::load(XMLElement * element)
{
	XMLElement* textures = element->FirstChildElement("textures");
	if (textures != nullptr)
	{
		XMLElement* data = textures->FirstChildElement();
		while (data != nullptr)
		{
			if (const char* name = data->Attribute("name"))
			{
				auto asset = AssetManager::instance().getAsset(name);
				if (asset != nullptr && asset->isA(TextureAsset::getClassHashCode()))
				{
					auto texture_asset = std::static_pointer_cast<TextureAsset>(asset);
					if (auto type = data->Attribute("type"))
					{
						if (std::string(type) == "DIFFUSE")
						{
							m_diffuseTexture = texture_asset;
						}
						else if (std::string(type) == "SPECULAR")
						{
							m_specularTexture = texture_asset;
						}
					}
				}
			}
			data = data->NextSiblingElement();
		}
	}
	XMLElement* shineness = element->FirstChildElement("shineness");
	if (shineness != nullptr)
	{
		shineness->QueryFloatText(&m_shineness);
	}
}

void Material::use()
{
	if (auto dText = m_diffuseTexture.lock())
	{
		dText->use();
	}
	if (auto sText = m_specularTexture.lock())
	{
		sText->use();
	}
}

void Material::disable()
{
	if (auto dText = m_diffuseTexture.lock())
	{
		dText->disable();
	}
	if (auto sText = m_specularTexture.lock())
	{
		sText->disable();
	}
}

unsigned int Material::getDiffuseID() const
{
	if (auto dText = m_diffuseTexture.lock())
	{
		return dText->getID();
	}
	return 0;
}

unsigned int Material::getSpecularID() const
{
	if (auto sText = m_specularTexture.lock())
	{
		return sText->getID();
	}
	return 0;
}

unsigned int Material::getDiffuseUnit() const
{
	if (auto dText = m_diffuseTexture.lock())
	{
		return dText->getUnit();
	}
	return 0;
}

unsigned int Material::getSpecularUnit() const
{
	if (auto sText = m_specularTexture.lock())
	{
		return sText->getUnit();
	}
	return 0;
}

float Material::getShineness() const
{
	return m_shineness;
}

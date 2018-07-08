#include "Core.h"
#include "TextureAsset.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

IMPLEMENT_DYNAMIC_CLASS(TextureAsset);

TextureAsset::TextureAsset()
	: m_loaded(false),
	m_id(0),
	m_unit(0)
{
}

TextureAsset::~TextureAsset()
{
	if (m_loaded)
	{
		glDeleteTextures(1, &m_id);
	}
	m_loaded = false;
}

void TextureAsset::load(tinyxml2::XMLElement * element)
{
	Asset::load(element);
	tinyxml2::XMLElement* path = element->FirstChildElement("path");
	if (path != nullptr)
	{
		int width;
		int height;
		int components;
		unsigned char* image_data = stbi_load(path->GetText(), &width, &height, &components, 4);

		if (image_data != nullptr)
		{
			glGenTextures(1, &m_id);
			glBindTexture(GL_TEXTURE_2D, m_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

			stbi_image_free(image_data);
			m_loaded = true;
		}
	}

	element->QueryUnsignedAttribute("unit", &m_unit);
}

void TextureAsset::initialize()
{
	Asset::initialize();
}

void TextureAsset::use()
{
	if (m_loaded)
	{
		glActiveTexture(GL_TEXTURE0 + m_unit);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}

void TextureAsset::setUnit(unsigned int unit)
{
	m_unit = unit;
}

unsigned int TextureAsset::getID() const
{
	return GL_TEXTURE0 + m_unit;
}

unsigned int TextureAsset::getUnit() const
{
	return m_unit;
}

void TextureAsset::disable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

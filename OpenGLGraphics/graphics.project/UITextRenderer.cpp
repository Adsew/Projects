#include "Core.h"

#include "AssetManager.h"
#include "TextureAsset.h"
#include "CameraComponent.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "UITextRenderer.h"

// Text default draw size
#define TEXT_BASE_WIDTH 0.05f
#define TEXT_BASE_HEIGHT 0.05f

// Sprite sheet numbers
#define CHAR_ACROSS 16
#define SHEET_DIMENSION 256
#define CHAR_DIMENSION 16
#define CHAR_TEXTURE_DIMENSION 1.0f / 16.0f;

IMPLEMENT_DYNAMIC_CLASS(UITextRenderer);

UITextRenderer::UITextRenderer()
	:
	m_position(0.0f, 0.0f),
	m_colour(1.0f, 1.0f, 1.0f),
	m_text(" "),
	m_size(1),
	m_configured(false),
	awaitingBind(false)
{

}

void UITextRenderer::initialize()
{
	// generate a vao and vbo
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// do not allow other objects to edit mesh
	glBindVertexArray(0);

	m_shaderProgram = std::make_unique<ShaderProgram>();
	m_shaderProgram->loadManual("UITextureShaderVS", "UITextureShaderFS");
	m_shaderProgram->initialize();

	m_texture = std::static_pointer_cast<TextureAsset>(AssetManager::instance().getAsset("ASCIISheet"));

	RenderManager::instance().attachUIRenderable(std::static_pointer_cast<IRenderable>(shared_from_this()), false);

	setText(m_text);

	m_configured = true;
}

void UITextRenderer::load(tinyxml2::XMLElement* element)
{
	Component::load(element);
	XMLElement *node = element->FirstChildElement("text");
	if (node != NULL)
	{
		m_text = node->GetText();
	}
	node = element->FirstChildElement("size");
	if (node != NULL)
	{
		int temp;
		node->QueryIntText(&temp);
		m_size = (unsigned int)temp;
	}
	node = element->FirstChildElement("position");
	if (node != NULL)
	{
		XMLElement *axis = node->FirstChildElement("x");
		if (axis != NULL)
		{
			axis->QueryFloatText(&m_position.x);
		}
		axis = node->FirstChildElement("y");
		if (axis != NULL)
		{
			axis->QueryFloatText(&m_position.y);
		}
	}
	node = element->FirstChildElement("colour");
	if (node != NULL)
	{
		XMLElement *axis = node->FirstChildElement("r");
		if (axis != NULL)
		{
			axis->QueryFloatText(&m_colour.x);
		}
		axis = node->FirstChildElement("g");
		if (axis != NULL)
		{
			axis->QueryFloatText(&m_colour.y);
		}
		axis = node->FirstChildElement("b");
		if (axis != NULL)
		{
			axis->QueryFloatText(&m_colour.z);
		}
	}
}

void UITextRenderer::update(float _deltaTime)
{
	if (awaitingBind)
	{
		bakeBuffer();
	}
}

void UITextRenderer::render()
{
	if (m_configured)
	{
		if (m_shaderProgram == nullptr)
		{
			LOG("UITextRenderer found nullptr ShaderProgram");
			return;
		}
		if (m_texture.expired())
		{
			LOG("UITextRenderer found expired/nonexistant Texture");
			return;
		}
		m_shaderProgram->use();

		if (auto &tex = m_texture.lock())
		{
			tex->use();
		}

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
		glBindVertexArray(0);

		m_shaderProgram->disable();
	}
}

void UITextRenderer::bakeBuffer()
{
	// start using the vao
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(TextureVertex), &m_vertices[0], GL_STATIC_DRAW);

	// VERTEX POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), nullptr);

	// VERTEX COLOUR
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), (GLvoid*)offsetof(TextureVertex, colour));

	// VERTEX TEXTURE
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), (GLvoid*)offsetof(TextureVertex, texture));

	// do not allow other objects to edit mesh
	glBindVertexArray(0);

	awaitingBind = false;
}

glm::vec2 UITextRenderer::findCharacterSpriteLocation(char c)
{
	glm::vec2 result(0, 0);
	int ascii = (int)c;
	int across = ascii % 16;
	int down = ascii / 16;

	across = across * CHAR_DIMENSION;
	down = down * CHAR_DIMENSION;

	result.x = (float)across / SHEET_DIMENSION;
	result.y = (float)down / SHEET_DIMENSION;

	return result;
}

void UITextRenderer::setText(std::string &text) {

    int line = 0;
    int space = 0;
	bool wasEmpty = false;

	m_vertices.clear();

	if (text.size() == 0)
	{
		text = " ";
		wasEmpty = true;
	}
	for (unsigned int i = 0; i < text.size(); i++)
	{
		TextureVertex v1, v2, v3, v4, v5, v6;

        if (text[i] == '\n')
        {
            line++;
            space = 0;
        }
        else
        {
            glm::vec2 textLoc = findCharacterSpriteLocation(text[i]);

            v1.position = glm::vec3(m_position, 0.0f);  // Top left corner
            v1.position.x += TEXT_BASE_WIDTH * m_size * space;
            v1.position.y -= TEXT_BASE_HEIGHT * m_size * line;
            v1.colour = m_colour;
            v1.texture = textLoc;

            v2.position = glm::vec3(m_position, 0.0f);  // Bottom left corner
            v2.position.x += TEXT_BASE_WIDTH * m_size * space;
            v2.position.y = v2.position.y - TEXT_BASE_HEIGHT * m_size - TEXT_BASE_HEIGHT * m_size * line;
            v2.colour = m_colour;
            v2.texture = textLoc;
            v2.texture.y += CHAR_TEXTURE_DIMENSION;

            v3.position = glm::vec3(m_position, 0.0f);  // Top right corner
            v3.position.x += TEXT_BASE_WIDTH * m_size * (space + 1);
            v3.position.y -= TEXT_BASE_HEIGHT * m_size * line;
            v3.colour = m_colour;
            v3.texture = textLoc;
            v3.texture.x += CHAR_TEXTURE_DIMENSION;

            v4.position = glm::vec3(m_position, 0.0f);  // Top right corner
            v4.position.x += TEXT_BASE_WIDTH * m_size * (space + 1);
            v4.position.y -= TEXT_BASE_HEIGHT * m_size * line;
            v4.colour = m_colour;
            v4.texture = textLoc;
            v4.texture.x += CHAR_TEXTURE_DIMENSION;

            v5.position = glm::vec3(m_position, 0.0f);  // Bottom left corner
            v5.position.x += TEXT_BASE_WIDTH * m_size * space;
            v5.position.y = v5.position.y - TEXT_BASE_HEIGHT * m_size - TEXT_BASE_HEIGHT * m_size * line;
            v5.colour = m_colour;
            v5.texture = textLoc;
            v5.texture.y += CHAR_TEXTURE_DIMENSION;

            v6.position = glm::vec3(m_position, 0.0f);  // Bottom right corner
            v6.position.x += TEXT_BASE_WIDTH * m_size * (space + 1);
            v6.position.y = v6.position.y - TEXT_BASE_HEIGHT * m_size - TEXT_BASE_HEIGHT * m_size * line;
            v6.colour = m_colour;
            v6.texture = textLoc;
            v6.texture.x += CHAR_TEXTURE_DIMENSION;
            v6.texture.y += CHAR_TEXTURE_DIMENSION;

            m_vertices.push_back(v1);
            m_vertices.push_back(v2);
            m_vertices.push_back(v3);
            m_vertices.push_back(v4);
            m_vertices.push_back(v5);
            m_vertices.push_back(v6);

            space++;
        }
	}

	if (wasEmpty)
	{
		text = "";
	}

	m_text = text;

	awaitingBind = true;
}

void UITextRenderer::setPosition(glm::vec2 &position)
{
	m_position = position;
}

void UITextRenderer::setColour(glm::vec3 &colour)
{
	m_colour = colour;

	for (TextureVertex &v : m_vertices)
	{
		v.colour = m_colour;
	}

	awaitingBind = true;
}

void UITextRenderer::setSize(unsigned int size)
{
	m_size = size;

	setText(m_text);
}

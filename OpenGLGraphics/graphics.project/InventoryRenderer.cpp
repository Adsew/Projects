#include "Core.h"
#include "InventoryRenderer.h"

#include "RenderManager.h"

#include "TextureAsset.h"
#include "ShaderProgram.h"

IMPLEMENT_DYNAMIC_CLASS(InventoryRenderer);

void InventoryRenderer::reloadInventory()
{


	// start using the vao
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_coords.size() * sizeof(ScreenCoordinates), &m_coords[0], GL_STATIC_DRAW);

	// VERTEX POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenCoordinates), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenCoordinates), (GLvoid*)offsetof(ScreenCoordinates, texCoord));

	// do not allow other objects to edit mesh
	glBindVertexArray(0);
}

std::size_t InventoryRenderer::panelCount() const
{
	return m_panels.size();
}

InventoryRenderer::InventoryRenderer()
{
	// generate a vao and vbo
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
}

InventoryRenderer::~InventoryRenderer()
{
	if (isInitialize())
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}
}

void InventoryRenderer::initialize()
{
	Component::initialize();


	m_screenWidth = RenderManager::instance().getWidth();
	m_screenHeight = RenderManager::instance().getHeight();

	assert(m_inventoryWidth <= m_screenWidth);
	assert(m_inventoryHeight <= m_screenHeight);

	RenderManager::instance().attachUIRenderable(std::static_pointer_cast<IRenderable>(shared_from_this()), false);
}

void InventoryRenderer::render()
{
	if (!m_coords.empty())
	{
		m_shaderProgram->use();
		unsigned int count = 0;
		for (auto& panel : m_panels)
		{
			if (count >= m_coords.size())
			{
				break;
			}
			if (auto texture = panel.texture.lock())
			{
				texture->use();
			}
			glBindVertexArray(m_vao);
			glDrawArrays(GL_TRIANGLES, count, 6);
			glBindVertexArray(0);
			count += 6;
		}

		m_shaderProgram->disable();
	}
}

void InventoryRenderer::load(XMLElement * element)
{
	element->QueryIntAttribute("width", &m_inventoryHeight);
	element->QueryIntAttribute("height", &m_inventoryWidth);
	element->QueryIntAttribute("item-width", &m_itemWidth);
	element->QueryIntAttribute("item-height", &m_itemHeight);
	element->QueryIntAttribute("spacing", &m_spacing);

	if (auto xmlShader = element->FirstChildElement(TO_STRING(ShaderProgram)))
	{
		m_shaderProgram = std::make_unique<ShaderProgram>();
		m_shaderProgram->load(xmlShader);
		m_shaderProgram->initialize();
	}
}

void InventoryRenderer::addInventoryIcon(const STRCODE code, std::weak_ptr<TextureAsset> textAsset)
{
	if (!textAsset.expired())
	{
		UIPanel panel;
		if (auto texture = textAsset.lock())
		{
			panel.uuid = code;
			panel.texture = texture;
		}
		auto it = std::find_if(m_panels.begin(), m_panels.end(), [&, panel](const auto& other) {
			return other.uuid == panel.uuid;
		});

		if (it != m_panels.end())
		{
			return;
		}

		glm::vec2 newPosition;
		if (!m_panels.empty())
		{
			newPosition = m_panels.at(0).position;
			for (auto panel : m_panels)
			{
				newPosition.x += m_itemWidth + m_spacing;
			}
		}
		else {
			glm::vec3 pos = getGameObject()->transform()->position();
			newPosition.x = pos.x;
			newPosition.y = pos.y;
		}



		panel.position = newPosition;

		m_panels.push_back(panel);

		ScreenCoordinates p1;
		// (top left)
		p1.position.x = (newPosition.x * 2) / m_screenWidth - 1;
		p1.position.y = (newPosition.y * 2) / m_screenHeight - 1;
		p1.texCoord = { 0, 0 };

		ScreenCoordinates p2;
		// (top right)
		p2.position.x = ((newPosition.x + m_itemWidth) * 2) / m_screenWidth - 1;
		p2.position.y = p1.position.y;
		p2.texCoord = { 1, 0 };

		// (bottom left)
		ScreenCoordinates p3;
		p3.position.x = p1.position.x;
		p3.position.y = ((newPosition.y - m_itemHeight) * 2) / m_screenHeight - 1;
		p3.texCoord = { 0, 1 };

		// (bottom right)
		ScreenCoordinates p4;
		p4.position.x = p2.position.x;
		p4.position.y = p3.position.y;
		p1.texCoord = { 1, 1 };


		m_coords.push_back(p1);		// top left
		m_coords.push_back(p3);		// bottom left
		m_coords.push_back(p2);		// top right
		m_coords.push_back(p2);		// top right
		m_coords.push_back(p3);		// bottom left
		m_coords.push_back(p4);		// bottom right

		reloadInventory();
	}
}
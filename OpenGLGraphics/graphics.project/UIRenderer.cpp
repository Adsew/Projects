#include "Core.h"

#include "CameraComponent.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "UIRenderer.h"

#define UI_HEIGHT 0.6f
#define UI_COLOUR 0.2f, 0.2f, 0.2f, 0.5f

IMPLEMENT_DYNAMIC_CLASS(UIRenderer);

void UIRenderer::initialize()
{
    // generate a vao and vbo
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

	RenderManager::instance().attachUIRenderable(std::static_pointer_cast<IRenderable>(shared_from_this()), true);

	m_configured = true;
}

void UIRenderer::load(tinyxml2::XMLElement* element)
{
	Component::load(element);

	XMLElement* shaderProgram = element->FirstChildElement(TO_STRING(ShaderProgram));
	if (shaderProgram != nullptr)
	{
		m_shaderProgram = std::make_unique<ShaderProgram>();
		m_shaderProgram->load(shaderProgram);
		m_shaderProgram->initialize();
	}
    
    XMLElement *node = element->FirstChildElement("visible");
    if (node != NULL)
    {
        node->QueryBoolText(&m_visible);
    }

    node = element->FirstChildElement("dimensions");
    if (node != NULL)
    {
        glm::vec2 tl, tr, bl, br;
        
        // Top left
        XMLElement *pos = node->FirstChildElement("tl");
        if (pos != NULL)
        {
            XMLElement *coord = pos->FirstChildElement("x");
            if (coord != NULL)
            {
                coord->QueryFloatText(&tl.x);
            }
            coord = pos->FirstChildElement("y");
            if (coord != NULL)
            {
                coord->QueryFloatText(&tl.y);
            }
        }

        // Bottom left
        pos = node->FirstChildElement("bl");
        if (pos != NULL)
        {
            XMLElement *coord = pos->FirstChildElement("x");
            if (coord != NULL)
            {
                coord->QueryFloatText(&bl.x);
            }
            coord = pos->FirstChildElement("y");
            if (coord != NULL)
            {
                coord->QueryFloatText(&bl.y);
            }
        }

        //Top right
        pos = node->FirstChildElement("tr");
        if (pos != NULL)
        {
            XMLElement *coord = pos->FirstChildElement("x");
            if (coord != NULL)
            {
                coord->QueryFloatText(&tr.x);
            }
            coord = pos->FirstChildElement("y");
            if (coord != NULL)
            {
                coord->QueryFloatText(&tr.y);
            }
        }

        // Bottom right
        pos = node->FirstChildElement("br");
        if (pos != NULL)
        {
            XMLElement *coord = pos->FirstChildElement("x");
            if (coord != NULL)
            {
                coord->QueryFloatText(&br.x);
            }
            coord = pos->FirstChildElement("y");
            if (coord != NULL)
            {
                coord->QueryFloatText(&br.y);
            }
        }

        setPosition(tl, bl, tr, br);
    }
}

void UIRenderer::render()
{
    if (m_awaitingBind)
    {
        bind();
    }
    
    if (m_configured && m_visible)
	{
		if (m_shaderProgram == nullptr)
		{
			LOG("UIRenderer found nullptr ShaderProgram");
			return;
		}

		glDisable(GL_DEPTH_TEST);

		m_shaderProgram->use();

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
		glBindVertexArray(0);

		m_shaderProgram->disable();

		glEnable(GL_DEPTH_TEST);
	}
}

void UIRenderer::bind()
{
    // start using the vao
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(SimpleVertex), &m_vertices[0], GL_STATIC_DRAW);

    // VERTEX POSITION
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), nullptr);

    // VERTEX COLOUR
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (GLvoid*)offsetof(SimpleVertex, colour));

    // do not allow other objects to edit mesh
    glBindVertexArray(0);
}

void UIRenderer::setPosition(glm::vec2 &tl, glm::vec2 &bl, glm::vec2 &tr, glm::vec2 &br)
{
    SimpleVertex v1, v2, v3, v4, v5, v6;

    v1.position = glm::vec3(tl, 0.0f);
    v1.colour = glm::vec4(UI_COLOUR);

    v2.position = glm::vec3(tr, 0.0f);
    v2.colour = glm::vec4(UI_COLOUR);

    v3.position = glm::vec3(br, 0.0f);
    v3.colour = glm::vec4(UI_COLOUR);

    v4.position = glm::vec3(br, 0.0f);
    v4.colour = glm::vec4(UI_COLOUR);

    v5.position = glm::vec3(tl, 0.0f);
    v5.colour = glm::vec4(UI_COLOUR);

    v6.position = glm::vec3(bl, 0.0f);
    v6.colour = glm::vec4(UI_COLOUR);

    m_vertices.clear();
    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
    m_vertices.push_back(v4);
    m_vertices.push_back(v5);
    m_vertices.push_back(v6);

    m_awaitingBind = true;
}

void UIRenderer::setVisible(bool b)
{
    m_visible = b;
}

bool UIRenderer::isVisible()
{
    return m_visible;
}

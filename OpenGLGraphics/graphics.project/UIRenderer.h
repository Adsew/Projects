#pragma once

#include "ShaderProgram.h"
#include "IRenderable.h"

struct SimpleVertex
{
    glm::vec3 position;
    glm::vec4 colour;
};

class UIRenderer : public IRenderable
{
    DECLARE_DYNAMIC_DERIVED_CLASS(UIRenderer, IRenderable);

protected:
    std::vector<SimpleVertex> m_vertices;
    unsigned int m_vao;
    unsigned int m_vbo;
    bool m_configured;
    bool m_visible;
    bool m_awaitingBind;
    std::unique_ptr<ShaderProgram> m_shaderProgram;

    void initialize() override;
    void load(tinyxml2::XMLElement* element) override;
    void render() override;
    void bind();
public:
    void setPosition(glm::vec2 &tl, glm::vec2 &bl, glm::vec2 &tr, glm::vec2 &br);
    void setVisible(bool b);
    bool isVisible();
};


#pragma once

#include "UIRenderer.h"

class TextureAsset;

struct TextureVertex 
{
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec2 texture;
};

class UITextRenderer : public IRenderable
{
    DECLARE_DYNAMIC_DERIVED_CLASS(UITextRenderer, IRenderable);

protected:
    std::vector<TextureVertex> m_vertices;
    unsigned int m_vao;
    unsigned int m_vbo;
    std::unique_ptr<ShaderProgram> m_shaderProgram;
    std::weak_ptr<TextureAsset> m_texture;
    bool m_configured;
    bool awaitingBind;

    glm::vec2 m_position;
    glm::vec3 m_colour;
    std::string m_text;
    unsigned int m_size;

public:
    UITextRenderer();
    void initialize() override;
    void load(tinyxml2::XMLElement* element) override;
    void update(float _deltaTime) override;
    void render() override;
    void bakeBuffer();
    glm::vec2 findCharacterSpriteLocation(char c);

    void setText(std::string &text);
    void setPosition(glm::vec2 &position);
    void setColour(glm::vec3 &colour);
    void setSize(unsigned int size);
};

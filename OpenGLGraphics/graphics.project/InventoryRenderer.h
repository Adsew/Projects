#pragma once

#include "IRenderable.h"

class TextureAsset;
class ShaderProgram;

struct ScreenCoordinates
{
	glm::vec2 position;
	glm::vec2 texCoord;
};

struct UIPanel
{
	STRCODE uuid;
	glm::vec2 position;
	std::weak_ptr<TextureAsset> texture;
	glm::vec2 __padding;
};


class InventoryRenderer : public IRenderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(InventoryRenderer, IRenderable);
private:
	std::vector<UIPanel> m_panels;
	std::vector<ScreenCoordinates> m_coords;
	int m_spacing;
	unsigned int m_vao;
	unsigned int m_vbo;

	int m_inventoryWidth;
	int m_inventoryHeight;
	int m_itemWidth;
	int m_itemHeight;

	std::unique_ptr<ShaderProgram> m_shaderProgram;

	void reloadInventory();

protected:
	int m_screenWidth;
	int m_screenHeight;


public:
	InventoryRenderer();
	~InventoryRenderer();
	void initialize() override;
	void render() override;
	void load(XMLElement* element) override;
	void addInventoryIcon(const STRCODE code, std::weak_ptr<TextureAsset> textAsset);
	std::size_t panelCount() const;
};
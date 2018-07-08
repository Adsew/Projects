#pragma once

#include "Asset.h"

// Asset for textures. Holds the texture ID and the unit you want to bind to
class TextureAsset : public Asset
{
	DECLARE_DYNAMIC_DERIVED_CLASS(TextureAsset, Asset);
private:
	bool m_loaded;
	unsigned int m_id;
	unsigned int m_unit;

public:
	TextureAsset();
	~TextureAsset();
	void load(tinyxml2::XMLElement* element) override;
	void initialize() override;
	void use() override;
	void disable() override;
	void setUnit(unsigned int unit);
	unsigned int getID() const;
	unsigned int getUnit() const;
};
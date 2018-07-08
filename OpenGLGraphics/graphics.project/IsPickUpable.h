#pragma once

#include "Component.h"

class TextureAsset;

class IsPickUpable : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(IsPickUpable, Component);
public:
	std::weak_ptr<TextureAsset> iconTexture;
	void load(XMLElement* element) override;
};
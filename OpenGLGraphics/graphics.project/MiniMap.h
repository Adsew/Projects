#pragma once

#include "InventoryRenderer.h"

class MiniMap : public InventoryRenderer
{
	DECLARE_DYNAMIC_DERIVED_CLASS(MiniMap, InventoryRenderer);
public:
	void load(XMLElement* element) override;
};
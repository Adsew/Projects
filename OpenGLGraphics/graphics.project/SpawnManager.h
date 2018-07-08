#pragma once

#include "Component.h"

class SpawnManager : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(SpawnManager, Component);
public:
	void load(XMLElement* element) override;
};
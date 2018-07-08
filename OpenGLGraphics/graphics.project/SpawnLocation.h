#pragma once

#include "Component.h"

class Spawnable;

class SpawnLocation : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(SpawnLocation, Component);
public:
	void initialize() override;
};
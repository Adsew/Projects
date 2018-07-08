#pragma once

#include "Component.h"

class IRenderable : public Component
{
	DECLARE_ABSTRACT_DERIVED_CLASS(IRenderable, Component);
private:
	friend class RenderManager;
	virtual void render() {};
public:
	IRenderable();
	virtual ~IRenderable();
};
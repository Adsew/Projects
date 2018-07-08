#pragma once

#include "Object.h"

// Base class for all assets in window
class Asset : public Object
{
	DECLARE_ABSTRACT_DERIVED_CLASS(Asset, Object);
public:
	Asset() = default;
	virtual ~Asset() = default;
	virtual void use() {};
	virtual void disable() {};
	// use the objects load and initialize
	using Object::load;
	using Object::initialize;
};
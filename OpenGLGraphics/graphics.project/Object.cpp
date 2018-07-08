#include "Core.h"
#include "Object.h"

IMPLEMENT_ABSTRACT_CLASS(Object);

Object::Object()
	: m_initialized(false)
{
	// Create UUID for saving later
	CreateUUID(&m_uuid);
	assert(m_uuid != GUID_NULL);
	m_uuidHashCode = GUIDToSTRCODE(m_uuid);
}


Object::~Object()
{
}

void Object::initialize()
{
	m_initialized = true;
}

void Object::update(float _deltaTime)
{
}

void Object::load(tinyxml2::XMLElement * element)
{
	if (auto str = element->Attribute("name"))
	{
		setName(str);
	}
}

bool Object::isInitialize() const
{
	return m_initialized;
}

const std::string & Object::getName() const
{
	return m_name;
}

const STRCODE Object::getNameHashCode() const
{
	return m_nameHashCode;
}

void Object::setName(const std::string & name)
{
	m_name = name;
	m_nameHashCode = getHashCode(name.c_str());
}

const STRCODE Object::getUUIDHashCode() const
{
	return m_uuidHashCode;
}

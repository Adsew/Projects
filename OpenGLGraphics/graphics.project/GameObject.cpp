#include "Core.h"
#include "GameObject.h"

#include <algorithm>

IMPLEMENT_DYNAMIC_CLASS(GameObject);

GameObject::GameObject()
	: m_transform(new Transform()),
	m_markedForDelete(false)
{
}


GameObject::~GameObject()
{
}

void GameObject::update(float _deltaTime)
{
	Object::update(_deltaTime);
	for (auto& c : m_components)
	{
		c->update(_deltaTime);
	}
	m_transform->update(_deltaTime);
}

void GameObject::initialize()
{
	Object::initialize();
}

void GameObject::load(tinyxml2::XMLElement * element)
{
	Object::load(element);

	bool foundTransform = false;

	tinyxml2::XMLElement* xmlTransform = element->FirstChildElement("Transform");
	if (xmlTransform != nullptr)
	{
		m_transform->m_gameObject = shared_from_this();
		m_transform->load(xmlTransform);
		m_transform->initialize();
	}
	else {
		LOG("Transform did not load from xml");
		m_transform->m_gameObject = shared_from_this();
		m_transform->initialize();
	}

	tinyxml2::XMLElement* components = element->FirstChildElement("Components");
	if (components == nullptr) { return; }
	tinyxml2::XMLElement* component = components->FirstChildElement();
	while (component != nullptr)
	{
		auto obj = CRtti::constructObject(component->Name());
		if (obj != nullptr && obj->isA(Component::getClassHashCode()))
		{
			auto comp = std::shared_ptr<Component>(static_cast<Component*>(obj));
			comp->m_gameObject = shared_from_this();
			comp->load(component);
			comp->initialize();
			m_components.push_back(comp);
		}

		component = component->NextSiblingElement();
	}
}

void GameObject::markForDelete()
{
	m_markedForDelete = true;
}

bool GameObject::isMarkedForDelete() const
{
	return m_markedForDelete;
}

std::shared_ptr<Transform> GameObject::transform()
{
	return m_transform;
}

void GameObject::addComponent(std::shared_ptr<Component> component)
{
	const STRCODE code = component->getDerivedClassHashCode();
	auto it = std::find_if(m_components.begin(), m_components.end(), [code](auto& c) {
		return code == c->getDerivedClassHashCode();
	});
	if (it == m_components.end())
	{
		component->m_gameObject = shared_from_this();
		m_components.emplace_back(component);
	}
}

void GameObject::removeComponent(const STRCODE code)
{
	m_components.remove_if([code](auto& c) {
		return c->getDerivedClassHashCode() == code;
	});
}

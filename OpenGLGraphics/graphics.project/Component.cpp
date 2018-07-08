#include "Core.h"
#include "Component.h"

IMPLEMENT_ABSTRACT_CLASS(Component);

Component::Component()
	: m_isFirstUpdate(true)
{

}

std::shared_ptr<GameObject> Component::getGameObject()
{
	return m_gameObject.lock();
}

void Component::update(float deltaTime)
{
	if (m_isFirstUpdate)
	{
		m_isFirstUpdate = false;
		start();
	}
}

void Component::start()
{
	// no-op
}

#pragma once

#include "Object.h"

// base class of all component type
// use for attaching component "scripts" to game objects to provide functionality to the GameObject
class Component : public Object, public std::enable_shared_from_this<Component>
{
	DECLARE_ABSTRACT_DERIVED_CLASS(Component, Object);
private:
	friend class GameObject;
	bool m_isFirstUpdate;

protected:
	std::weak_ptr<GameObject> m_gameObject;

public:
	Component();
	std::shared_ptr<GameObject> getGameObject();
	void update(float deltaTime) override;
	virtual void start();
	template <typename _Ty>
	std::shared_ptr<_Ty> getComponent() const;
	template <typename _Ty>
	std::shared_ptr<_Ty> findComponent() const;
};

template<typename _Ty>
inline std::shared_ptr<_Ty> Component::getComponent() const
{
	return m_gameObject->getComponent<_Ty>();
}

template<typename _Ty>
inline std::shared_ptr<_Ty> Component::findComponent() const
{
	return m_gameObject->findComponent<_Ty>();
}

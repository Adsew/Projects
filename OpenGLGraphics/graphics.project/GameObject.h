#pragma once

#include <functional>

#include "Component.h"
#include "Transform.h"

// power house of the window. holds components that pertain to object and the transform
class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
	DECLARE_DYNAMIC_BASE_CLASS(GameObject);
private:
	std::list<std::shared_ptr<Component>> m_components;
	std::shared_ptr<Transform> m_transform;	// hold the unique_ptr seperatley from the other components for faster retrieval
	bool m_markedForDelete;

public:
	GameObject();
	~GameObject();

	void update(float _deltaTime) override;
	void initialize() override;

	void load(tinyxml2::XMLElement* element) override;

	void markForDelete();
	bool isMarkedForDelete() const;

	std::shared_ptr<Transform> transform();

	//*****************************
	// GET COMPONENT
	//
	template <class _Ty>
	std::shared_ptr<_Ty> getComponent()
	{
		auto code = _Ty::getClassHashCode();
		auto it = std::find_if(m_components.cbegin(), m_components.cend(), [code](auto& component)
		{
			return code == component->getDerivedClassHashCode();
		});

		if (it != m_components.end())
		{
			return std::static_pointer_cast<_Ty>((*it));
		}
		return nullptr;
	}

	//*****************************
	//	ADD COMPONENT
	//
	// add component by pointer
	void addComponent(std::shared_ptr<Component> component);
	// add component by type
	template <class _Ty>
	std::shared_ptr<_Ty> addComponent()
	{
		Object* obj = CRtti::constructObject(_Ty::getClassHashCode());
		if (obj != nullptr)
		{
			auto comp = std::shared_ptr<Component>(static_cast<Component*>(obj));
			comp->m_gameObject = shared_from_this();
			m_components.push_back(comp);
			return std::static_pointer_cast<_Ty>(comp);
		}
		return nullptr;
	}

	template <class _Ty>
	std::shared_ptr<_Ty> findComponent()
	{
		const STRCODE code = _Ty::getClassHashCode();
		auto it = std::find_if(m_components.begin(), m_components.end(), [&, code](const auto& component) {
			return component->isA(code);
		});
		if (it != m_components.end())
		{
			return std::static_pointer_cast<_Ty>((*it));
		}
		return nullptr;
	}

	//**********************************
	//	REMOVE COMPONENT
	//
	// remove component by RTTI type
	void removeComponent(const STRCODE code);
};


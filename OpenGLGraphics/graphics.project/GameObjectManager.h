#pragma once

#include "IManager.h"

class GameObject;

// Database for all GameObjects in scene
class GameObjectManager : protected IManager
{
private:
	GameObjectManager();
	~GameObjectManager();
	std::list<std::shared_ptr<GameObject>> m_objects;
	std::weak_ptr<GameObject> m_activeCamera;

public:
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

	static GameObjectManager& instance()
	{
		static GameObjectManager _instance;
		return _instance;
	}

	void initialize() override;
	void update(float _deltaTime) override;
	std::shared_ptr<GameObject> getCamera();
	void setCamera(std::shared_ptr<GameObject> go);

	//*****************************
	// GET COMPONENTS WITH TYPE
	//
	template <class _Ty>
	std::list<std::shared_ptr<_Ty>> getComponentsWithType() const
	{
		std::list<std::shared_ptr<_Ty>> hasCompList;

		auto iter = m_objects.begin();

		while (iter != m_objects.end())
		{
			if (std::shared_ptr<_Ty> comp = (*iter)->getComponent<_Ty>()) {

				hasCompList.push_back(comp);
			}

			iter++;
		}

		return hasCompList;
	}
	std::shared_ptr<GameObject> findGameObject(const char* name) const;

	template <class _Ty>
	std::shared_ptr<_Ty> getComponent() const
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (auto comp = (*it)->getComponent<_Ty>())
			{
				return comp;
			}
		}
		return nullptr;
	}

	void addGameObject(std::shared_ptr<GameObject> go);
};


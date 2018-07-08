#pragma once

#include "IManager.h"

class Asset;

// Database the manages all assets used in the window
class AssetManager : public IManager
{
private:
	AssetManager();
	~AssetManager();
	std::list<std::shared_ptr<Asset>> m_assets;

public:
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

	static AssetManager& instance()
	{
		static AssetManager _instance;
		return _instance;
	}

	void update(float _deltaTime) override;
	void initialize() override;
	std::shared_ptr<Asset> getAsset(const std::string& name);
};


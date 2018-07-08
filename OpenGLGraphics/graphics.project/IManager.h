#pragma once

// interface for all managers
class IManager
{
protected:
	const std::string m_name;

	IManager(const char* name);
	virtual ~IManager();
	virtual void update(float _deltaTime) = 0;
	virtual void initialize() = 0;

public:
	IManager(const IManager&) = delete;
	IManager& operator=(const IManager&) = delete;
};


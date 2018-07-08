#pragma once

class Window;

class Engine final
{
private:
	Engine();
	~Engine();

	bool m_running;
public:
	static Engine& instance()
	{
		static Engine _instance;
		return _instance;
	}
	void gameLoop();
	void initialize();
	void requestShutdown();
};
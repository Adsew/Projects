#pragma once

#include "IManager.h"

class Light;
class ShaderProgram;
class DirectionalLight;
class SpotLight;
class Window;
class IRenderable;

// RenderManager
class RenderManager final : protected IManager
{
private:
	RenderManager();
	~RenderManager();
	std::unique_ptr<Window> m_window;
	std::list<std::weak_ptr<IRenderable>> m_renderables;
    std::list<std::weak_ptr<IRenderable>> m_ui_renderables;
	std::vector<std::weak_ptr<Light>> m_lights;	// requires contigious memory
	std::weak_ptr<DirectionalLight> m_directionalLight;	// only one directional light allowed at a time
    std::weak_ptr<SpotLight> m_spotLight;

public:
	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager&) = delete;

	static RenderManager& instance()
	{
		static RenderManager _instance;
		return _instance;
	}

	void update(float _deltaTime) override;
	void initialize() override;
	// primarly called from the input manager callback
	void resize(int width, int height);
	void attachRenderable(std::shared_ptr<IRenderable> obj);
    void RenderManager::attachUIRenderable(std::shared_ptr<IRenderable> obj, bool front);
    void detachRenderable(std::shared_ptr<IRenderable> obj);
    void detachUIRenderable(std::shared_ptr<IRenderable> obj);
	const int getWidth() const;
	const int getHeight() const;
	const Window* getWindow() const;

	void addLight(std::shared_ptr<Light> light);
	void bindLights(ShaderProgram* program);
};


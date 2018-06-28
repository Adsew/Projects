#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

/*
Student: James Brooks
Class: Game Architecture

File: RenderSystem.h

Class: RenderSystem

Description: A system responsable for rendering renderable objects to the screen.
*/


#include "ISystem.h"


class IRenderable;

namespace sf {
    class RenderWindow;
}


class RenderSystem : public ISystem {

    friend class GameEngine;


    /*****Variables *****/

private:
	
    sf::RenderWindow *window;

    std::vector<std::list<IRenderable *>> renderComponents;

    std::string winTitle;

    int winHeight, winWidth;


    /***** Functions *****/

private:

    RenderSystem();

    ~RenderSystem();

    RenderSystem(const RenderSystem &rs) = default;

    RenderSystem &operator=(RenderSystem &rs) = default;

protected:

    virtual void initialize() override;

    virtual void clean() override;

    virtual void update() override;

public:	
	
    inline static RenderSystem& Instance() {

		static RenderSystem instance;
		return instance;
	}
	
    void addRenderable(IRenderable* component);
	
	void removeRenderable(IRenderable* component);

    sf::RenderWindow *getWindow();

    void setWindowSize(int w, int h);
};

#endif // !RENDER_SYSTEM_H

#ifndef I_RENDERABLE_H
#define I_RENDERABLE_H

/*
Student: James Brooks
Class: Game Architecture

File: IRenderable.h

Class: IRenderable

Description: Interface to provide screen rendering functionality.
*/


#define UI_HIGHEST_PRIORITY 0
#define UI_MID_PRIORITY 1
#define UI_LOW_PRIORITY 2

#define HIGHEST_PRIORITY 3
#define HIGH_PRIORITY 4
#define MID_PRIORITY 5
#define LOW_PRIORITY 6
#define LOWEST_PRIORITY 7

#define PRIORITIES_START 0
#define PRIORITIES 8


class IRenderable {

	friend class RenderSystem;

    /***** Variables *****/

private:

    unsigned int priority;


    /***** Functions *****/

protected:	
	
    IRenderable();

    IRenderable(unsigned int p);

    virtual ~IRenderable();

    // Called to render the object to the window
	virtual void render() = 0;

public:

    // Set the priority the object will render over others
    void setRenderPriority(unsigned int p);

    // Get the priority the object will render over others
    unsigned int getRenderPriority() const;
};

#endif // !I_RENDERABLE_H

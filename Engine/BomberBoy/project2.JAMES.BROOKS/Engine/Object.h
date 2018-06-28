#ifndef OBJECT_H
#define OBJECT_H

/*
Student: James Brooks
Class: Game Architecture

File: Object.h

Class: Object

Description: Base class for most entities persisting in the game world.
*/


class Object {


    /**** Variables *****/

private:
	
    static unsigned int uniqueIDCount;

    unsigned int id;

    bool initialized;
    bool needsDeletion;


    /***** Functions *****/

protected:
	
    Object(unsigned int uniqueID);

    virtual ~Object();
	
public:

    // Return true if id's are the same
    bool operator==(const Object &obj);

    // Initialize object to a usable state
	virtual void initialize() = 0;

    // Allows user to perform any cleanup operations before destruction
    virtual void onDestroy() = 0;

    // Return true if object has been initialized
    bool isInitialized();

    // Set an object to be deleted by the manager
    virtual void destroy() final;

    // Special use to undo a destroy before the object is destroyed
    // Mainly just used by managers when needed
    virtual void undestroy() final;

    // Check if object is set for deletion
    virtual bool pollNeedsDeletion() final;

    // One time call to initialize the ID generator for objects
    static void initializeIDs();

    // Generate a unique ID
    static unsigned int generateID();
};

#endif // !OBJECT_H

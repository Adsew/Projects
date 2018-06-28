/*
Student: James Brooks
Class: Game Architecture

File: Object.cpp

Class: Object

Description: Base class for most entities persisting in the game world.
*/


#include "Core.h"

#include "Object.h"


unsigned int Object::uniqueIDCount = 0;


Object::Object(unsigned int uniqueID) {

    id = uniqueID;

    initialized = false;
    needsDeletion = false;
}

Object::~Object() {}

// Return true if id's are the same
bool Object::operator==(const Object &obj) {

    if (id == obj.id) {

        return true;
    }

    return false;
}

// Return true if object has been initialized
bool Object::isInitialized() {
    
    return initialized;
}

// Set an object to be deleted by the manager
void Object::destroy() {

    needsDeletion = true;
}

// Special use to undo a destroy before the object is destroyed
// Mainly just used by managers when needed
void Object::undestroy() {

    needsDeletion = false;
}

// Check if object is set for deletion
bool Object::pollNeedsDeletion() {

    return needsDeletion;
}

// One time call to initialize the ID generator for objects
void Object::initializeIDs() {

    Object::uniqueIDCount = 0;
}

// Generate a unique ID
unsigned int Object::generateID() {

    return Object::uniqueIDCount++;
}

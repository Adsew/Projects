/*
Student: James Brooks
Class: Game Architecture

File: Asset.cpp

Class: Asset

Description: A base class for all future assets of any time to be created.
*/


#include "Core.h"

#include "Asset.h"


Asset::Asset(unsigned int uniqueID) : Object(uniqueID) {


}

Asset::~Asset() {


}

void Asset::setName(const char *n) {

    name = n;
}

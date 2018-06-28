#ifndef CORE_H
#define CORE_H

/*
Student: James Brooks
Class: Game Architecture

File: Core.h

Description: Precompiled header for the common files and support macros.
*/


// Basic standard libraries

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <memory>


// Extra libraries

#include "Timer.h"
#include "FileSystem.h"
#include "RTTI.h"


// Easy debug output

#ifdef _DEBUG
    #define DEBUG_LOG(__out__) std::cout << __out__ << std::endl;
#else
    #define DEBUG_LOG(__out__)
#endif


#endif // !CORE_H

/*
Student: James Brooks
Class: Game Architecture

File: FileSystem.cpp

Class: FileSystem

Description: Manages the input and output of files used by the game.
    The code is modified from my previous project to allow for multiple
    simultaneous file use via the FileAccessor sub class.
*/


#include "Core.h"

#include "RenderSystem.h"
#include "Timer.h"
#include "SceneManager.h"
#include "tinyxml2.h"
#include "FileSystem.h"


using namespace tinyxml2;


    // Class Functions

FileSystem::FileSystem() {

    initialized = false;
}

FileSystem::FileSystem(FileSystem &fs) {}

FileSystem::~FileSystem() {

    for (std::map<std::string, XMLDocument *>::iterator iter = assets.begin();
        iter != assets.end();
        iter++) {

        delete (*iter).second;
        (*iter).second = NULL;
    }
}

FileSystem &FileSystem::operator=(FileSystem &fs) { return fs; }

// Initialize the file system to a usable state
bool FileSystem::initialize(const char *settingsLoc) {

    XMLDocument settingsFile;
    settingsFile.LoadFile(settingsLoc);

    if (settingsFile.Error() == false) {

        XMLNode *settings = settingsFile.FirstChildElement("Settings");

        if (settings != NULL) {

            // Get game engine settings
            XMLElement *windowNode = settings->FirstChildElement("Window");

            if (windowNode != NULL) {

                if (windowNode->Attribute("width") != NULL && windowNode->Attribute("height") != NULL) {

                    RenderSystem::Instance().setWindowSize(windowNode->IntAttribute("width"), windowNode->IntAttribute("height"));
                }
                if (windowNode->Attribute("fps") != NULL) {

                    Timer::Instance().setFPS(windowNode->FloatAttribute("fps"));
                }
            }

            // Asset files
            XMLElement *assetsNode = settings->FirstChildElement("AssetFiles");

            if (assetsNode != NULL) {

                XMLDocument *assetFile = NULL;

                for (XMLElement *asset = assetsNode->FirstChildElement();
                    asset != NULL;
                    asset = asset->NextSiblingElement()) {

                    if (asset->Attribute("file") != NULL && asset->Attribute("name") != NULL) {

                        assetFile = new XMLDocument();

                        assetFile->LoadFile(asset->Attribute("file"));

                        if (assetFile->Error() == false) {

                            assets[asset->Attribute("name")] = assetFile;
                        }
                        else {

                            DEBUG_LOG("FileSystem: Initialization load failure. Can not open asset " << asset->Attribute("file") << ".");

                            delete assetFile;

                            assetFile = NULL;
                        }
                    }
                }
            }

            // Scene files
            XMLElement *scenesNode = settings->FirstChildElement("Scenes");

            if (scenesNode != NULL) {

                XMLDocument *sceneFile = NULL;

                for (XMLElement *scene = scenesNode->FirstChildElement();
                    scene != NULL;
                    scene = scene->NextSiblingElement()) {

                    if (scene->Attribute("file") != NULL && scene->Attribute("name") != NULL) {

                        sceneFile = new XMLDocument();

                        sceneFile->LoadFile(scene->Attribute("file"));

                        if (sceneFile->Error() == false) {

                            assets[scene->Attribute("name")] = sceneFile;

                            SceneManager::Instance().addScene(scene->Attribute("name"));

                            if (scene->Attribute("default") != NULL) {
                                if (scene->BoolAttribute("default")) {

                                    SceneManager::Instance().changeScene(scene->Attribute("name")); // On first update will load this scene
                                }
                            }
                        }
                        else {

                            DEBUG_LOG("FileSystem: Initialization load failure. Can not open scene " << scene->Attribute("file") << ".");

                            delete sceneFile;

                            sceneFile = NULL;
                        }
                    }
                }
            }
        }
    }

    if (assets.size() > 0) {

        initialized = true;

        return initialized;
    }

    return false;
}

// Initialize the file system to a usable state
void FileSystem::initialize(const std::string &settingsLoc) {

    this->initialize(settingsLoc.c_str());
}

// Set the file to be used for loading
std::unique_ptr<FileSystem::FileAccessor> FileSystem::useFile(const char *fileRef) {

    try {
        // Generate and return a unique pointer to the desired doc if exists
        std::unique_ptr<FileSystem::FileAccessor> access(new FileSystem::FileAccessor(assets.at(fileRef)));

        return access;
    }
    catch (...) {}

    return NULL;
}

// Set the file to be used for loading
std::unique_ptr<FileSystem::FileAccessor> FileSystem::useFile(const std::string &fileRef) {

    return this->useFile(fileRef.c_str());
}


    // Loading Functions

// Changes from current node to a contained element
bool FileSystem::traverseToElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *elem) {

    if (accessor->activeAsset != NULL) {

        XMLElement *temp = accessor->activeElem;

        if (accessor->activeElem == NULL) {

            accessor->activeElem = accessor->activeAsset->FirstChildElement(elem);
        }
        else {

            accessor->activeElem = accessor->activeElem->FirstChildElement(elem);
        }

        if (accessor->activeElem != NULL) {

            return true;
        }
        else {

            accessor->activeElem = temp;
        }
    }

    return false;
}

// Changes from current node to a contained element
bool FileSystem::traverseToElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &elem) {

    return this->traverseToElement(accessor, elem.c_str());
}

// Chamges from current node to the first child
bool FileSystem::traverseToChildElement(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    if (accessor->activeAsset != NULL) {

        XMLElement *temp = accessor->activeElem;

        if (accessor->activeElem == NULL) {

            accessor->activeElem = accessor->activeAsset->FirstChildElement();
        }
        else {

            accessor->activeElem = accessor->activeElem->FirstChildElement();
        }

        if (accessor->activeElem != NULL) {

            return true;
        }
        else {

            accessor->activeElem = temp;
        }
    }

    return false;
}

// Changes from current element to the next element of same parent
bool FileSystem::traverseToSyblingElement(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    if (accessor->activeAsset != NULL) {
        if (accessor->activeElem != NULL) {

            XMLElement *temp = accessor->activeElem;

            accessor->activeElem = accessor->activeElem->NextSiblingElement();

            if (accessor->activeElem != NULL) {

                return true;
            }
            else {

                accessor->activeElem = temp;
            }
        }
    }

    return false;
}

// Changes from current element to the next element of same parent by name
bool FileSystem::traverseToSyblingElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *syb) {

    if (accessor->activeAsset != NULL) {
        if (accessor->activeElem != NULL) {

            XMLElement *temp = accessor->activeElem;

            accessor->activeElem = accessor->activeElem->NextSiblingElement(syb);

            if (accessor->activeElem != NULL) {

                return true;
            }
            else {

                accessor->activeElem = temp;
            }
        }
    }

    return false;
}

// Changes from current element to the next element of same parent by name
bool FileSystem::traverseToSyblingElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &syb) {

    return this->traverseToSyblingElement(accessor, syb.c_str());
}

// Return from current tag to parent tag
bool FileSystem::traverseToParentElement(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    if (accessor->activeElem != NULL) {

        XMLElement *temp = accessor->activeElem;

        XMLNode *parent = accessor->activeElem->Parent();

        accessor->activeElem = (XMLElement *)parent;

        if (accessor->activeElem != NULL) {

            return true;
        }
        else {

            accessor->activeElem = temp;
        }
    }

    return false;
}

// Get the text contained in the tags of the current element
bool FileSystem::getElementText(std::unique_ptr<FileSystem::FileAccessor> &accessor, std::string &val) {

    if (accessor->activeElem != NULL) {
        
        val = accessor->activeElem->GetText();

        return true;
    }

    return false;
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, std::string &val) {

    if (accessor->activeElem != NULL) {
        if (accessor->activeElem->Attribute(name) != 0) {

            val = accessor->activeElem->Attribute(name);

            return true;
        }
    }

    return false;
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, std::string &val) {

    return this->getAttribute(accessor, name.c_str(), val);
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, int &val) {

    if (accessor->activeElem != NULL) {
        if (accessor->activeElem->Attribute(name) != 0) {

            val = accessor->activeElem->IntAttribute(name);

            return true;
        }
    }

    return false;
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, int &val) {

    return this->getAttribute(accessor, name.c_str(), val);
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, float &val) {

    if (accessor->activeElem != NULL) {
        if (accessor->activeElem->Attribute(name) != 0) {

            val = accessor->activeElem->FloatAttribute(name);

            return true;
        }
    }

    return false;
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, float &val) {

    return this->getAttribute(accessor, name.c_str(), val);
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, double &val) {

    if (accessor->activeElem != NULL) {
        if (accessor->activeElem->Attribute(name) != 0) {

            val = accessor->activeElem->DoubleAttribute(name);

            return true;
        }
    }

    return false;
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, double &val) {

    return this->getAttribute(accessor, name.c_str(), val);
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, bool &val) {

    if (accessor->activeElem != NULL) {
        if (accessor->activeElem->Attribute(name) != 0) {

            val = accessor->activeElem->BoolAttribute(name);

            return true;
        }
    }

    return false;
}

// Get an attribute from the current element, if exists
bool FileSystem::getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, bool &val) {

    return this->getAttribute(accessor, name.c_str(), val);
}


    // Saving Functions

    // Save the given file to the location/filename
bool FileSystem::saveFile(const char *refName, const char *fileName) {

    if (assets.find(refName) != assets.end()) {

        if (assets[refName]->SaveFile(fileName) == XML_SUCCESS) {

            return true;
        }
    }

    return false;
}

// Save the given file to the location/filename
bool FileSystem::saveFile(const std::string &refName, const std::string &fileName) {

    return this->saveFile(refName.c_str(), fileName.c_str());
}

    // Create a new asset file. Is not automatically saved
std::unique_ptr<FileSystem::FileAccessor> FileSystem::createTempFile(const char *refName) {

    if (assets.find(refName) == assets.end()) {

        XMLDocument *asset = new XMLDocument();

        if (asset->Error() == false) {

            std::unique_ptr<FileSystem::FileAccessor> access(new FileSystem::FileAccessor(asset));

            assets[refName] = asset;

            return access;
        }
        else {

            delete asset;
        }
    }

    return NULL;
}

// Create a new asset file. Is not automatically saved
std::unique_ptr<FileSystem::FileAccessor> FileSystem::createTempFile(const std::string &refName) {

    return this->createTempFile(refName.c_str());
}

// Load an existing file from location, returns false if doesn't exist
std::unique_ptr<FileSystem::FileAccessor> FileSystem::loadFile(const char *refName, const char *fileLoc) {

    // Ensure doesn't already exist
    if (assets.find(refName) == assets.end()) {

        XMLDocument *asset = new XMLDocument();

        asset->LoadFile(fileLoc);

        if (asset->Error() == false) {

            std::unique_ptr<FileSystem::FileAccessor> access(new FileSystem::FileAccessor(asset));

            assets[refName] = asset;

            return access;
        }
        else {

            delete asset;
        }
    }

    return false;
}

// Load an existing file from location, returns false if doesn't exist
std::unique_ptr<FileSystem::FileAccessor> FileSystem::loadFile(const std::string &refName, const std::string &fileLoc) {

    return this->loadFile(refName.c_str(), fileLoc.c_str());
}

// Create a new element as a child of the current element
bool FileSystem::newElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name) {

    if (accessor->activeAsset != NULL) {

        XMLElement *newElem = accessor->activeAsset->NewElement(name);

        if (newElem != NULL) {

            if (accessor->activeElem != NULL) {

                accessor->activeElem->InsertEndChild(newElem);
            }
            else {

                accessor->activeAsset->InsertEndChild(newElem);
            }
            
            accessor->activeElem = newElem;

            return true;
        }
    }

    return false;
}

// Create a new element as a child of the current element
bool FileSystem::newElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name) {

    return this->newElement(accessor, name.c_str());
}

// Add text to the current element
bool FileSystem::setElementText(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *text) {

    if (accessor->activeElem != NULL) {

        accessor->activeElem->SetText(text);

        return true;
    }

    return false;
}

// Add text to the current element
bool FileSystem::setElementText(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &text) {

    return this->setElementText(accessor, text.c_str());
}

bool FileSystem::destroyCurrentElement(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    if (accessor->activeElem != NULL) {

        XMLElement *temp = accessor->activeElem;

        XMLNode *parent = accessor->activeElem->Parent();

        accessor->activeElem = (XMLElement *)parent;

        // If the parent was NULL, the node is the doc, the final parent node, cant delete it
        if (accessor->activeElem != NULL) {

            temp->DeleteChildren();
            accessor->activeAsset->DeleteNode(temp);
            
            return true;
        }
        else {

            accessor->activeElem = temp;
        }
    }

    return false;
}

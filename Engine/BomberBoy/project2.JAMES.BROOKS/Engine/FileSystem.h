#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

/*
Student: James Brooks
Class: Game Architecture

File: FileSystem.h

Class: FileSystem

Description: Manages the input and output of files used by the game.
    The code is modified from my previous project to allow for multiple
    simultaneous file use via the FileAccessor sub class.
*/


namespace tinyxml2 {
    class XMLDocument;
    class XMLElement;
}


class FileSystem {

public:

    // Allows unique access to multiple files at once through multiple iterators
    class FileAccessor {

        friend class FileSystem;

    protected:

        tinyxml2::XMLDocument * const activeAsset;
        tinyxml2::XMLElement *activeElem;

        bool usable;

    protected:

        FileAccessor(tinyxml2::XMLDocument *doc) : activeAsset(doc), activeElem(NULL) {
            
            usable = false;

            if (activeAsset != NULL) {

                usable = true;
            }
        }
    };

private:

    /***** Variables *****/

    std::map<std::string, tinyxml2::XMLDocument *> assets;

    bool initialized;


    /****** Functions *****/

    FileSystem();

    FileSystem(FileSystem &fs);

    ~FileSystem();

    FileSystem &operator=(FileSystem &fs);

public:

        // Class Functions

    inline static FileSystem &Instance() {

        static FileSystem instance;

        return instance;
    }

    // Initialize the file system to a usable state
    bool initialize(const char *settingsLoc);

    // Initialize the file system to a usable state
    void initialize(const std::string &settingsLoc);


        // File access functions

    // Set the file to be used for loading
    std::unique_ptr<FileSystem::FileAccessor> useFile(const char *fileRef);

    // Set the file to be used for loading
    std::unique_ptr<FileSystem::FileAccessor> useFile(const std::string &fileRef);


        // Loading Functions

    // Changes from current node to a contained element
    bool traverseToElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *elem);

    // Changes from current node to a contained element
    bool traverseToElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &elem);

    // Chamges from current node to the first child
    bool traverseToChildElement(std::unique_ptr<FileSystem::FileAccessor> &accessor);

    // Changes from current element to the next element of same parent
    bool traverseToSyblingElement(std::unique_ptr<FileSystem::FileAccessor> &accessor);

    // Changes from current element to the next element of same parent by name
    bool traverseToSyblingElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *syb);

    // Changes from current element to the next element of same parent by name
    bool traverseToSyblingElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &syb);

    // Return from current tag to parent tag
    bool traverseToParentElement(std::unique_ptr<FileSystem::FileAccessor> &accessor);

    // Get the text contained in the tags of the current element
    bool getElementText(std::unique_ptr<FileSystem::FileAccessor> &accessor, std::string &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, std::string &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, std::string &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, int &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, int &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, float &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, float &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, double &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, double &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name, bool &val);

    // Get an attribute from the current element, if exists
    bool getAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name, bool &val);


        // Saving Functions

    // Save the given file to the location/filename
    bool saveFile(const char *refName, const char *fileName);

    // Save the given file to the location/filename
    bool saveFile(const std::string &refName, const std::string &fileName);

    // Create a new asset file. Is not automatically saved
    std::unique_ptr<FileSystem::FileAccessor> createTempFile(const char *refName);

    // Create a new asset file. Is not automatically saved
    std::unique_ptr<FileSystem::FileAccessor> createTempFile(const std::string &refName);

    // Load an existing file from location, returns false if doesn't exist
    std::unique_ptr<FileSystem::FileAccessor> loadFile(const char *refName, const char *fileLoc);

    // Load an existing file from location, returns false if doesn't exist
    std::unique_ptr<FileSystem::FileAccessor> loadFile(const std::string &refName, const std::string &fileLoc);

    // Create a new element as a child of the current element
    bool newElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *name);
    
    // Create a new element as a child of the current element
    bool newElement(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &name);

    // Add text to the current element
    bool setElementText(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *text);

    // Add text to the current element
    bool setElementText(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &text);

    // Add an attribute to the current element
    template <typename T>
    bool setElementAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const char *attribute, const T val) {

        if (activeElem != NULL) {

            activeElem->SetAttribute(attribute, val);

            return true;
        }

        return false;
    }

    // Add an attribute to the current element
    template <typename T>
    bool setElementAttribute(std::unique_ptr<FileSystem::FileAccessor> &accessor, const std::string &attribute, const T val) {

        return this->setElementAttribute(attribute.c_str(), val);
    }

    
        // Extra xml handling

    bool destroyCurrentElement(std::unique_ptr<FileSystem::FileAccessor> &accessor);
};

#endif

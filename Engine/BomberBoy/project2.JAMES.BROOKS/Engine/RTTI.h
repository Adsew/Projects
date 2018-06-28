#ifndef RTTI_H
#define RTTI_H

/*
Student: James Brooks
Class: Game Architecture

File: RTTI.h

Description: Macros for creating dynamic type definitions so components can be created
    with the use of strings
*/


// Generate dynamic component types

#define REGISTER_COMPONENT(_class_type_)                    \
                                                            \
    friend class ComponentManager;                          \
    friend class GameObject;                                \
                                                            \
protected:                                                  \
                                                            \
    _class_type_##(unsigned int uniqueID);                  \
                                                            \
    _class_type_##(unsigned int uniqueID, const char *type);\
                                                            \
    virtual ~##_class_type_##() = default;                  \
                                                            \
public:                                                     \
                                                            \
    static Component *createComponent(unsigned int id) {    \
                                                            \
        return new _class_type_(id);                        \
    }                                                       \
                                                            \
    class __TypeRegister {                                  \
                                                            \
    public:                                                 \
                                                            \
        __TypeRegister() {                                  \
                                                            \
            ComponentManager::Instance().addComponentType(  \
                #_class_type_,                              \
                &##_class_type_##::createComponent          \
            );                                              \
        }                                                   \
    };



#define REGISTER_ABSTRACT_COMPONENT(_class_type_)           \
                                                            \
    friend class ComponentManager;                          \
    friend class GameObject;                                \
                                                            \
protected:                                                  \
                                                            \
    _class_type_##(unsigned int uniqueID, const char *type);\
                                                            \
    virtual ~##_class_type_##() = default;



#define CORE_IMPLEMENT_COMPONENT(_class_type_) _class_type_##::__TypeRegister _class_type_##_tr;



#define IMPLEMENT_COMPONENT(_class_type_)                                   \
                                                                            \
static _class_type_##::__TypeRegister _class_type_##_tr;                    \
                                                                            \
_class_type_##::##_class_type_##(unsigned int uniqueID)                     \
    : Component(uniqueID, #_class_type_) {}                                 \
                                                                            \
_class_type_##::##_class_type_##(unsigned int uniqueID, const char *type)   \
    : Component(uniqueID, type) {}



#define IMPLEMENT_ABSTRACT_COMPONENT(_class_type_)                          \
                                                                            \
_class_type_##::##_class_type_##(unsigned int uniqueID, const char *type)   \
    : Component(uniqueID, type) {}



#define IMPLEMENT_DERIVED_COMPONENT(_derived_type_, _base_type_)        \
                                                                        \
static _derived_type_##::__TypeRegister _derived_type_##_tr;            \
                                                                        \
_derived_type_##::##_derived_type_##(unsigned int uniqueID)             \
    : _base_type_##(uniqueID, #_derived_type_ ) {}                      \
                                                                        \
_derived_type_::_derived_type_(unsigned int uniqueID, const char *type) \
    : _base_type_##(uniqueID, type) {}



// Generate dynamic asset types

#define REGISTER_ASSET(_class_type_)                        \
                                                            \
public:                                                     \
                                                            \
    friend class AssetManager;                              \
                                                            \
    static Asset *createAsset(unsigned int id) {            \
                                                            \
        return new _class_type_(id);                        \
    }                                                       \
                                                            \
    class __TypeRegister {                                  \
                                                            \
    public:                                                 \
                                                            \
        __TypeRegister() {                                  \
                                                            \
            AssetManager::Instance().addAssetType(          \
                #_class_type_,                              \
                &##_class_type_##::createAsset              \
            );                                              \
        }                                                   \
    };


#define CORE_IMPLEMENT_ASSET(_class_type_) _class_type_##::__TypeRegister _class_type_##_tr;


#define IMPLEMENT_ASSET(_class_type_) static _class_type_##::__TypeRegister _class_type_##_tr;


#endif // !RTTI
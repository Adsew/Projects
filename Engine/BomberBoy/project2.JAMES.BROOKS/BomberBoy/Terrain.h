#ifndef TERRAIN_H
#define TERRAIN_H

/*
Student: James Brooks
Class: Game Architecture

File: Terrain.h

Class: Terrain

Description: Keeps track of map data for the current level.
*/


#include "Component.h"


class Terrain : public Component {

    REGISTER_COMPONENT(Terrain);

    friend class LevelData;


    /***** Variables *****/

protected:

    // Parallel maps, terrain is ground, objects are collidable data
    std::vector<std::vector<GameObject *>> terrain;
    std::vector<std::vector<GameObject *>> colliders;
    std::vector<std::vector<GameObject *>> destructables;
    std::vector<std::vector<GameObject *>> powerups;
    std::vector<std::vector<GameObject *>> entities;

    std::list<std::pair<int, int>> spawnPoints;

    std::string mapFile;

    int tileWidth, tileHeight;


    /***** Functions *****/

protected:
    
    virtual void initialize() override;

    virtual void update() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

    // Create a tile gameobject with the given properties
    GameObject *createTile(const char *tileAssetName, int w, int h, int xOrig, int yOrig, int xPos, int yPos, bool collider, bool destructable, unsigned int renderPriority);

    // Unload tiles held by the map, if any
    void unloadTiles();

public:

    virtual Component &operator=(const Component &comp) override;

    // Load a map file and create the tiles needed
    void loadMapFile(const char *mapFile);

    // Take the given entity and place him in the map grid
    bool placeEntityOnMap(GameObject *entity, int posX, int posY);

    // Take the given entity and give it the correct position without tracking it on the grid
    bool placeEntityOnMapNoCollision(GameObject *entity, int posX, int posY);

    // Remove an entity from the map, making it none collidable
    bool removeEntityFromMap(int posX, int posY);

    // Check if you can move to the given square, then move the game object
    bool requestMoveEntity(Transform *trans, int posX, int posY, int distX, int distY);

    // Check if an entity or collider is at the given coordinates and pass a reference if so
    GameObject *checkCollisionOnMap(int posX, int posY);

    // Remove the game object at the given coordinates from the map and destroy the object
    bool removeAndDestroyObject(int posX, int posY);

    // Get the file name the map loaded from
    std::string &getMapFile();
};

#endif // !TERRAIN_H

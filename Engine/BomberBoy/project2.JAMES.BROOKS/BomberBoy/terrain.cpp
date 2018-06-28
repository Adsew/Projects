/*
Student: James Brooks
Class: Game Architecture

File: Terrain.cpp

Class: Terrain

Description: Keeps track of map data for the current level.
*/


#include "Core.h"

#include <sstream>

#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "Tile.h"
#include "Sprite.h"
#include "Transform.h"
#include "terrain.h"


#define SPAWN_POINT_ID 15


IMPLEMENT_COMPONENT(Terrain);


void Terrain::initialize() {

    tileWidth = 0;
    tileHeight = 0;
}

void Terrain::update() {


}

void Terrain::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    if (FileSystem::Instance().getAttribute(accessor, "mapfile", mapFile)) {

        loadMapFile(mapFile.c_str());
    }
}

// Create a tile gameobject with the given properties
GameObject *Terrain::createTile(const char *tileAssetName, int w, int h, int xOrig, int yOrig, int xPos, int yPos, bool collider, bool destructable, unsigned int renderPriority) {

    GameObject *tile = GameObjectManager::Instance().createGameObject();
    Tile *tileComp = (Tile *)ComponentManager::Instance().createComponent("Tile");
    Sprite *tileSprite = (Sprite *)ComponentManager::Instance().createComponent("Sprite");
    Transform *tileTrans = tile->getTransform();

    tile->setName("Tile");
    tile->setScene(this->gameObject);

    tileComp->setCollidable(collider);
    tileComp->setDestructable(destructable);

    tileSprite->setRectangle(w, h, xOrig, yOrig);
    tileSprite->setTextureAsset(tileAssetName);
    tileSprite->setRenderPriority(renderPriority);

    tileTrans->setPosition((float)xPos, (float)yPos);

    tile->addComponent(tileComp);
    tile->addComponent(tileSprite);

    return tile;
}

// Unload tiles held by the map, if any
void Terrain::unloadTiles() {

    for (unsigned int i = 0; i < terrain.size(); i++) {
        for (unsigned int j = 0; j < terrain[i].size(); j++) {

            if (terrain[i][j] != NULL) {

                terrain[i][j]->destroy();
            }

            if (colliders[i][j] != NULL) {

                colliders[i][j]->destroy();
            }

            if (destructables[i][j] != NULL) {

                destructables[i][j]->destroy();
            }

            if (powerups[i][j] != NULL) {

                powerups[i][j]->destroy();
            }
        }

        terrain[i].clear();
        colliders[i].clear();
        destructables[i].clear();
        powerups[i].clear();
    }

    terrain.clear();
    colliders.clear();
    destructables.clear();
    powerups.clear();
}

Component &Terrain::operator=(const Component &comp) {

    return *this;
}

// Load a map file and create the tiles needed
void Terrain::loadMapFile(const char *mapFile) {

    std::unique_ptr<FileSystem::FileAccessor> mapFileAccessor;
    
    if (!(mapFileAccessor = FileSystem::Instance().useFile(mapFile))) {

        mapFileAccessor = FileSystem::Instance().loadFile(mapFile, mapFile);
    }

    if (mapFileAccessor != NULL) {

        this->mapFile = mapFile;

        unloadTiles();

        if (FileSystem::Instance().traverseToElement(mapFileAccessor, "map")) {

            std::string tileset = "";
            int tilesAcross = 0, tilesDown = 0;
            int mapWidth = 0, mapHeight = 0;

            FileSystem::Instance().getAttribute(mapFileAccessor, "width", mapWidth);
            FileSystem::Instance().getAttribute(mapFileAccessor, "height", mapHeight);

            FileSystem::Instance().getAttribute(mapFileAccessor, "tilewidth", tileWidth);
            FileSystem::Instance().getAttribute(mapFileAccessor, "tileheight", tileHeight);

            // Get tileset data
            if (FileSystem::Instance().traverseToElement(mapFileAccessor, "tileset")) {

                FileSystem::Instance().getAttribute(mapFileAccessor, "texture", tileset);
                FileSystem::Instance().getAttribute(mapFileAccessor, "width", tilesAcross);
                FileSystem::Instance().getAttribute(mapFileAccessor, "height", tilesDown);
            }

            // Load underlying terrain such as the grass
            if (FileSystem::Instance().traverseToSyblingElement(mapFileAccessor, "layer")) {

                std::string data = "";

                FileSystem::Instance().traverseToElement(mapFileAccessor, "data");

                FileSystem::Instance().getElementText(mapFileAccessor, data);

                std::stringstream dataStream(data);
                char ignore = ' ';

                // Iterate and fill map with tiles based on data
                terrain.resize(mapHeight);
                entities.resize(mapHeight);

                for (unsigned int i = 0; i < terrain.size(); i++) {

                    terrain[i].resize(mapWidth);
                    entities[i].resize(mapWidth);

                    for (unsigned int j = 0; j < terrain[i].size(); j++) {

                        int x = 0, y = 0, tileNum = 0;

                        dataStream >> tileNum >> ignore;

                        if (tileNum != 0) {

                            if (tileNum == SPAWN_POINT_ID) {

                                spawnPoints.push_back(std::pair<int, int>(j, i));
                            }

                            tileNum--;
                            x = (tileNum % tilesAcross) * tileWidth;
                            y = ((int)(tileNum / tilesAcross)) * tileHeight;

                            terrain[i][j] = createTile(tileset.c_str(), tileWidth, tileHeight, x, y, j * tileWidth, i * tileHeight, false, false, LOWEST_PRIORITY);
                        }
                        else {

                            terrain[i][j] = NULL;
                        }
                    }
                }

                FileSystem::Instance().traverseToParentElement(mapFileAccessor);
            }

            // load the collidable terrain
            if (FileSystem::Instance().traverseToSyblingElement(mapFileAccessor)) {

                std::string data = "";

                FileSystem::Instance().traverseToElement(mapFileAccessor, "data");

                FileSystem::Instance().getElementText(mapFileAccessor, data);

                std::stringstream dataStream(data);
                char ignore = ' ';

                // Iterate and fill map with tiles based on data
                colliders.resize(mapHeight);

                for (unsigned int i = 0; i < colliders.size(); i++) {

                    colliders[i].resize(mapWidth);

                    for (unsigned int j = 0; j < colliders[i].size(); j++) {

                        int x = 0, y = 0, tileNum = 0;

                        dataStream >> tileNum >> ignore;

                        if (tileNum != 0) {

                            tileNum--;
                            x = (tileNum % tilesAcross) * tileWidth;
                            y = ((int)(tileNum / tilesAcross)) * tileHeight;

                            colliders[i][j] = createTile(tileset.c_str(), tileWidth, tileHeight, x, y, j * tileWidth, i * tileHeight, true, false, LOW_PRIORITY);
                        }
                        else {

                            colliders[i][j] = NULL;
                        }
                    }
                }

                FileSystem::Instance().traverseToParentElement(mapFileAccessor);
            }

            // load the powers
            if (FileSystem::Instance().traverseToSyblingElement(mapFileAccessor)) {

                std::string data = "";

                FileSystem::Instance().traverseToElement(mapFileAccessor, "data");

                FileSystem::Instance().getElementText(mapFileAccessor, data);

                std::stringstream dataStream(data);
                char ignore = ' ';

                // Iterate and fill map with tiles based on data
                powerups.resize(mapHeight);

                for (unsigned int i = 0; i < powerups.size(); i++) {

                    powerups[i].resize(mapWidth);

                    for (unsigned int j = 0; j < powerups[i].size(); j++) {

                        int x = 0, y = 0, tileNum = 0;

                        dataStream >> tileNum >> ignore;

                        if (tileNum != 0) {

                            tileNum--;
                            x = (tileNum % tilesAcross) * tileWidth;
                            y = ((int)(tileNum / tilesAcross)) * tileHeight;

                            powerups[i][j] = createTile(tileset.c_str(), tileWidth, tileHeight, x, y, j * tileWidth, i * tileHeight, false, false, LOW_PRIORITY);
                        }
                        else {

                            powerups[i][j] = NULL;
                        }
                    }
                }

                FileSystem::Instance().traverseToParentElement(mapFileAccessor);
            }

            // load the destructable terrain
            if (FileSystem::Instance().traverseToSyblingElement(mapFileAccessor)) {

                std::string data = "";

                FileSystem::Instance().traverseToElement(mapFileAccessor, "data");

                FileSystem::Instance().getElementText(mapFileAccessor, data);

                std::stringstream dataStream(data);
                char ignore = ' ';

                // Iterate and fill map with tiles based on data
                destructables.resize(mapHeight);

                for (unsigned int i = 0; i < destructables.size(); i++) {

                    destructables[i].resize(mapWidth);

                    for (unsigned int j = 0; j < destructables[i].size(); j++) {

                        int x = 0, y = 0, tileNum = 0;

                        dataStream >> tileNum >> ignore;

                        if (tileNum != 0) {

                            tileNum--;
                            x = (tileNum % tilesAcross) * tileWidth;
                            y = ((int)(tileNum / tilesAcross)) * tileHeight;

                            destructables[i][j] = createTile(tileset.c_str(), tileWidth, tileHeight, x, y, j * tileWidth, i * tileHeight, true, true, LOW_PRIORITY);
                        }
                        else {

                            destructables[i][j] = NULL;
                        }
                    }
                }

                FileSystem::Instance().traverseToParentElement(mapFileAccessor);
            }
        }
    }
}

// Take the given entity and place him in the map grid
bool Terrain::placeEntityOnMap(GameObject *entity, int posX, int posY) {

    if (posY >= 0 && posY < (int)entities.size() 
        && posX >= 0 && posX < (int)entities[0].size()
        && entity != NULL
        ) {

        if (entities[posY][posX] == NULL) {

            Transform *trans = entity->getTransform();

            if (trans != NULL) {

                trans->position.x = (float)posX * tileWidth;
                trans->position.y = (float)posY * tileHeight;
            }

            entities[posY][posX] = entity;

            return true;
        }
    }

    return false;
}

// Take the given entity and give it the correct position without tracking it on the grid
bool Terrain::placeEntityOnMapNoCollision(GameObject *entity, int posX, int posY) {

    if (posY >= 0 && posY < (int)entities.size()
        && posX >= 0 && posX < (int)entities[0].size()
        && entity != NULL
        ) {

        Transform *trans = entity->getTransform();

        if (trans != NULL) {

            trans->position.x = (float)posX * tileWidth;
            trans->position.y = (float)posY * tileHeight;
        }

        return true;
    }

    return false;
}

// Remove an entity from the map, making it none collidable
bool Terrain::removeEntityFromMap(int posX, int posY) {

    if (posY >= 0 && posY < (int)entities.size()
        && posX >= 0 && posX < (int)entities[0].size()
        ) {

        if (entities[posY][posX] != NULL) {

            entities[posY][posX] = NULL;

            return true;
        }
    }

    return false;
}

// Check if you can move to the given square, then move the game object
bool Terrain::requestMoveEntity(Transform *trans, int posX, int posY, int distX, int distY) {

    if (entities[posY][posX] != NULL && trans != NULL) {
        if (entities[posY + distY][posX + distX] == NULL
            && colliders[posY + distY][posX + distX] == NULL
            && destructables[posY + distY][posX + distX] == NULL) {

            entities[posY + distY][posX + distX] = entities[posY][posX];
            entities[posY][posX] = NULL;

            trans->position.x = (float)(posX + distX) * tileWidth;
            trans->position.y = (float)(posY + distY) * tileHeight;

            return true;
        }
    }

    return false;
}

// Check if an entity is at the given coordinates and pass a reference if so
GameObject *Terrain::checkCollisionOnMap(int posX, int posY) {

    if (posY >= 0 && posY < (int)entities.size()
        && posX >= 0 && posX < (int)entities[0].size()
        && posY >= 0 && posY < (int)destructables.size()
        && posX >= 0 && posX < (int)destructables[0].size()
        && posY >= 0 && posY < (int)colliders.size()
        && posX >= 0 && posX < (int)colliders[0].size()
        ) {

        if (entities[posY][posX] != NULL) {

            return entities[posY][posX];
        }

        if (destructables[posY][posX] != NULL) {

            return destructables[posY][posX];
        }

        return colliders[posY][posX];
    }

    return NULL;
}

// Remove the game object at the given coordinates from the map and destroy the object
bool Terrain::removeAndDestroyObject(int posX, int posY) {

    if (posY >= 0 && posY < (int)entities.size()
        && posX >= 0 && posX < (int)entities[0].size()
        && posY >= 0 && posY < (int)destructables.size()
        && posX >= 0 && posX < (int)destructables[0].size()
        ) {

        if (entities[posY][posX] != NULL) {

            entities[posY][posX]->destroy();
            entities[posY][posX] = NULL;

            return true;
        }
        else if (destructables[posY][posX] != NULL) {

            destructables[posY][posX]->destroy();
            destructables[posY][posX] = NULL;

            return true;
        }
    }

    return false;
}

// Get the file name the map loaded from
std::string &Terrain::getMapFile() {

    return mapFile;
}

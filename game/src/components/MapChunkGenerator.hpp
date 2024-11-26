#pragma once
#include "Map.hpp"
#include "SpriteManager.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

class MapChunkGenerator{

public:
    MapChunkGenerator(Map& map, float cameraSpawnXCords);
    
    void insertObjectInMap(GameObject& gameObject);
    void updateMap(float cameraPosX);

private:
    void generateMapChunks();
    void fillMapChunk(MapChunk& mapChunk);

    int m_cameraChunkIndex;
    int m_lastChunkIndex;
    SpriteManager& m_spriteManager = SpriteManager::getInstance();
    Map& m_map;    
};
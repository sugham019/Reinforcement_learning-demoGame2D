#include "MapChunkGenerator.hpp"
#include "ErrorHandler.hpp"
#include "Map.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace{

int convertCordsToChunk(float xCords){
    if(xCords < 1){
        reportError("Coordinates cannot be negative");
    }
    return xCords / CHUNK_WIDTH;
}

}

MapChunkGenerator::MapChunkGenerator(Map& map, float cameraSpawnXCords)
: m_map(map), m_cameraChunkIndex(convertCordsToChunk(cameraSpawnXCords)){
    if(m_cameraChunkIndex == 0){
        reportError("Camera can not spawn at first chunk of the map");
    }        
    m_map.activeChunkRange = sf::Vector2i(m_cameraChunkIndex-1, m_cameraChunkIndex +(CHUNK_BUFFER_SIZE - 2));
    generateMapChunks();
}

void MapChunkGenerator::insertObjectInMap(GameObject& gameObject){
    const int chunk = convertCordsToChunk(gameObject.getPosInMap().x);
    if(chunk < m_map.activeChunkRange.x || chunk > m_map.activeChunkRange.y){
        reportError("Coordinates out of map range");
    }
    m_map.mapChunks[chunk % CHUNK_BUFFER_SIZE].gameObjects.push_back(&gameObject);
}

void MapChunkGenerator::updateMap(float cameraPosX){
 
    const int currentCameraChunk = convertCordsToChunk(cameraPosX);
    if(currentCameraChunk > m_cameraChunkIndex){
        const int offset = currentCameraChunk - m_cameraChunkIndex;
        m_map.activeChunkRange.y += offset;
        m_map.activeChunkRange.x += offset;

        const int index = m_map.activeChunkRange.y % CHUNK_BUFFER_SIZE;
        MapChunk mapChunk{m_map.activeChunkRange.y};
        fillMapChunk(mapChunk);
        m_map.mapChunks[index] = mapChunk;
        m_cameraChunkIndex = currentCameraChunk;
    }
}

void MapChunkGenerator::generateMapChunks(){
    const int startChunk = m_cameraChunkIndex -1;

    for(int i=0; i<CHUNK_BUFFER_SIZE; i++){
        MapChunk mapChunk{startChunk+i};
        fillMapChunk(mapChunk);

        const int index = (startChunk + i) % CHUNK_BUFFER_SIZE;
        m_map.mapChunks[index] = mapChunk;
    }
    m_map.activeChunkRange.x = startChunk;
    m_map.activeChunkRange.y = startChunk + (CHUNK_BUFFER_SIZE-1);
}

void MapChunkGenerator::fillMapChunk(MapChunk& mapChunk){
    const float baseXCords = mapChunk.chunkNum * CHUNK_WIDTH;
    const sf::Vector2f tilePosInMap(baseXCords+70, WINDOW_HEIGHT-60);
    GameObject* tile = new GameObject(m_spriteManager.tile, tilePosInMap, 
            true, GameObjectType::TILE);

    mapChunk.gameObjects.push_back(tile);

    const sf::Vector2f lavaPosInMap(baseXCords, WINDOW_HEIGHT-30);
    GameObject* lava = new GameObject(m_spriteManager.lava, lavaPosInMap, 
            true, GameObjectType::LAVA);

    mapChunk.gameObjects.push_back(lava);

    if(mapChunk.chunkNum == 1){ // Skip spike generation for player spawn chunk
        return;
    }

    int randomNum = 70 + rand() % 231;
    const sf::Vector2f spikePosInMap(baseXCords + randomNum, WINDOW_HEIGHT-90);
    GameObject* spike = new GameObject(m_spriteManager.spike, spikePosInMap, 
            true, GameObjectType::SPIKE);
    
    
    mapChunk.gameObjects.push_back(spike);
}
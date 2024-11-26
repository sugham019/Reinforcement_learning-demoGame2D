#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "../entities/GameObject.hpp"
#include <SFML/System/Vector2.hpp>
#include <list>
#include "Window.hpp"
#include <sys/types.h>

constexpr unsigned int CHUNK_WIDTH = 300;
constexpr unsigned int CHUNK_BUFFER_SIZE = (WINDOW_WIDTH / CHUNK_WIDTH) + 2;

struct MapChunk{
    int chunkNum;
    std::list<GameObject*> gameObjects;

    MapChunk& operator=(const MapChunk& chunk) {
        for(GameObject* gameObject: gameObjects){
            delete gameObject;
        }
        chunkNum = chunk.chunkNum;
        gameObjects = std::move(chunk.gameObjects);
        return *this;
    }
};

struct Map{

    MapChunk mapChunks[CHUNK_BUFFER_SIZE];
    sf::Vector2i activeChunkRange;
}; 

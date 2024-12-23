#include "Game.hpp"
#include "components/CollisionDetector.hpp"
#include <SFML/System/Clock.hpp>
#include "components/ErrorHandler.hpp"
#include <iostream>
#include "components/Map.hpp"
#include "components/MapChunkGenerator.hpp"
#include "components/Renderer.hpp"
#include "components/RewardSender.hpp"
#include "components/SpriteManager.hpp"
#include "components/Window.hpp"
#include "entities/Bullet.hpp"
#include "entities/GameObject.hpp"
#include "entities/Player.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>

Game::Game(): m_eventHandler(*this), m_rewardSender("rl_game1"){
    init();
}


Game::~Game(){
    delete m_player;
    delete m_window;
}

void Game::init(){
    m_window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
    m_collisionDetector = new AABBCollisionDetector();
    setupPlayerAndMap();
    startGameLoop();
}

void Game::setupPlayerAndMap(){
    const sf::Vector2f playerSpawn(CHUNK_WIDTH+70, 0.7 * WINDOW_HEIGHT);
    m_camera = playerSpawn;
    m_player = new Player(m_spriteManager.player, playerSpawn, 100, 20, true);
    m_player->lockCamera(m_camera);
    m_mapChunkGenerator = new MapChunkGenerator(m_map, m_camera.x);
    m_mapChunkGenerator->insertObjectInMap(*m_player);
}

void Game::startGameLoop(){

    Renderer renderer(*m_window);
    const sf::Vector2f fallDown(0,0.2f);
    const sf::Vector2f jumpUp(0, -0.2f);
    const sf::Vector2f moveLeft(m_player->getMovementSpeed(), 0);
    bool firstStart = true;

    while (m_window->isOpen()){

        if(m_player->isDead()){
            restart();
            m_isPaused = true;
            std::cout << "Press Enter to restart" << std::endl;
        }

        sf::Event event;
        while (m_window->pollEvent(event)){
            m_eventHandler.submitEvent(event);
        }

        if(!m_isPaused){

            updateBulletPos();
            if(m_jump>0){
                updateObjectPosition(*m_player, jumpUp);
                m_jump += jumpUp.y;
            }else{
                updateObjectPosition(*m_player, fallDown);
            }
            updateObjectPosition(*m_player, moveLeft);
            m_mapChunkGenerator->updateMap(m_camera.x);
        }

        m_window->clear();
        renderer.draw(m_camera, m_map);
        m_window->display();
    }
}

void Game::updateBulletPos(){
    if(m_player->activeBullet == nullptr){
        return;
    }
    Bullet& bullet = *m_player->activeBullet;
    const sf::Vector2f& bulletDirection = bullet.getDirection();

    sf::Vector2f movement(bulletDirection.x * bullet.getSpeed(), bulletDirection.y * bullet.getSpeed());
    updateObjectPosition(bullet, movement);
}

void Game::restart(){
    m_score = 0;
    delete m_player;
    setupPlayerAndMap();
}

void Game::updateObjectPosition(GameObject& gameObject, const sf::Vector2f& offset){

    if(gameObject.isRigid()){
        const sf::Vector2f destination = gameObject.getPosInMap() + offset;
        const int destinationChunkNum = destination.x / CHUNK_WIDTH;

        if(destinationChunkNum < m_map.activeChunkRange.x || destinationChunkNum > m_map.activeChunkRange.y){
            reportWarning("Cannot move object out of map range");
            return;
        }
        MapChunk& destinationChunk = m_map.mapChunks[destinationChunkNum % CHUNK_BUFFER_SIZE];
        
        for(GameObject* gameObject1: destinationChunk.gameObjects){
            if(m_collisionDetector->isObjectColliding(destination, gameObject.getSize(), 
                    gameObject1->getPosInMap(), gameObject1->getSize())){

                if(gameObject1 != &gameObject){
                    handleCollision(gameObject, *gameObject1);
                    return;
                }
            }
        }
    }

    gameObject.updatePosInMap(offset);
    const int chunk = gameObject.m_chunk;
    const int newChunk = gameObject.getPosInMap().x / CHUNK_WIDTH;

    if(chunk != newChunk){
        m_map.mapChunks[chunk % CHUNK_BUFFER_SIZE].gameObjects.remove(&gameObject);
        m_map.mapChunks[newChunk % CHUNK_BUFFER_SIZE].gameObjects.push_back(&gameObject);
        gameObject.m_chunk = newChunk;
    }
}

void Game::jump(){

    if(!m_isPlayerJumping){
        m_jump = m_player->getSize().y * 1.9;
        m_isPlayerJumping = true;
    }
}

void Game::shoot(){
    if(m_player->activeBullet == nullptr){
        sf::Vector2f direction(1, 0);
        sf::Vector2f pos = m_player->getPosInMap();
        pos.x += m_player->getSize().x + 1;
        pos.y += 20;
        m_player->activeBullet = new Bullet(m_spriteManager.bullet, pos, direction, 1.0f);
        m_mapChunkGenerator->insertObjectInMap(*m_player->activeBullet);
        if(!m_isPlayerJumping){
            m_rewardSender.sendReward(Reward::GOOD_SHOT);
        }else{
            m_rewardSender.sendReward(Reward::BAD_SHOT);
        }
    }
}

void Game::handleCollision(GameObject& gameObject1, GameObject& gameObject2){

    if(gameObject1.getType() == GameObjectType::PLAYER){

        if(gameObject2.getType() == GameObjectType::TILE){
            if(m_isPlayerJumping){
                m_isPlayerJumping = false;
            }
        }else if(gameObject2.getType() == GameObjectType::LAVA || gameObject2.getType() == GameObjectType::SPIKE){
            destroyObject(gameObject1);
            m_rewardSender.sendReward(Reward::DEAD);
        }
    }else if(gameObject1.getType() == GameObjectType::BULLET && gameObject2.getType() == GameObjectType::SPIKE){

        destroyObject(gameObject1);
        destroyObject(gameObject2);
        delete &gameObject1;
        delete &gameObject2;

        m_player->activeBullet = nullptr;
        destroyObject(gameObject2);
        m_rewardSender.sendReward(Reward::SPIKE_BLASTED);
    }
}

void Game::destroyObject(GameObject& gameObject){
    const int chunkNum = gameObject.getPosInMap().x / CHUNK_WIDTH;
    if(chunkNum < m_map.activeChunkRange.x || chunkNum > m_map.activeChunkRange.y){
        reportWarning("Object is not in active chunk so already destroyed");
        return;
    }
    if(gameObject.getType() == GameObjectType::PLAYER){
        m_player->destroy();
    }
    m_map.mapChunks[chunkNum % CHUNK_BUFFER_SIZE].gameObjects.remove(&gameObject);
}


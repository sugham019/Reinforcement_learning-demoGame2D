#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include "EventHandler.hpp"
#include "components/CollisionDetector.hpp"
#include "components/Keymap.hpp"
#include "components/Map.hpp"
#include "components/MapChunkGenerator.hpp"
#include "components/RewardSender.hpp"
#include "components/SpriteManager.hpp"
#include "entities/GameObject.hpp"
#include "entities/Player.hpp"

class Game{

public:
    Game();
    ~Game();

private:
    void init();
    void startGameLoop();
    void updateObjectPosition(GameObject& gameObject, const sf::Vector2f& offset);
    void updateBulletPos();
    void restart();
    void jump();
    void shoot();
    void handleCollision(GameObject& gameObject1, GameObject& gameObject2);    
    void destroyObject(GameObject& gameObject);
    void setupPlayerAndMap();

    SpriteManager& m_spriteManager = SpriteManager::getInstance();
    EventHandler m_eventHandler;
    sf::RenderWindow* m_window = nullptr;
    Keymap m_keymap = getDefaultKeymap();
    Player* m_player = nullptr;
    MapChunkGenerator* m_mapChunkGenerator = nullptr;
    CollisionDetector* m_collisionDetector = nullptr;
    RewardSender m_rewardSender;
    sf::Vector2f m_camera;
    sf::Clock m_clock;


    Map m_map;
    float m_score = 0;
    bool m_isPlayerJumping = false;
    float m_jump, m_deltaTime;

    static constexpr const char* WINDOW_NAME = "RL-Game";
    friend class EventHandler;
};
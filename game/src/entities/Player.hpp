#pragma once
#include "Bullet.hpp"
#include "Character.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

class Player: public Character{

public:
    Player(sf::Sprite& sprite, const sf::Vector2f& spawnPosition, 
            float healthPoints, float attackDamage, bool isRigid);

    const bool& isDead(){
        return m_isDead;
    }

    void destroy(){
        m_isDead = true;
    }

    Bullet* activeBullet = nullptr;

private:
    bool m_isDead = false;
};
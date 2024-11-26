#pragma once
#include "GameObject.hpp"
#include <SFML/System/Vector2.hpp>

class Bullet: public GameObject{

public:
    Bullet(sf::Sprite& sprite, const sf::Vector2f& posInMap, const sf::Vector2f& direction, float speed);
    void changeSpeed(float newSpeed);

    const float& getSpeed(){
        return m_speed;
    }

    const sf::Vector2f& getDirection(){
        return m_direction;
    }
    
private:
    const sf::Vector2f m_direction;
    float m_speed;
};
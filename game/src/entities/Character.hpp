#pragma once
#include "GameObject.hpp"
#include "../components/ErrorHandler.hpp"

class Character: public GameObject{

public:
    Character(float healthPoints, float attackDamage, 
        sf::Sprite& sprite, const sf::Vector2f& posInMap, bool isRigid, GameObjectType type, float movementSpeed);
    
    void increaseHealthPoints(float healthIncreament){
        m_healthPoints += healthIncreament;
    }

    void setMovementSpeed(float movementSpeed){
        if(movementSpeed < 0){
            reportWarning("Movement speed cannot be negative");
            return;
        }
        m_movementSpeed = movementSpeed;
    }

    const float& getMovementSpeed(){
        return m_movementSpeed;
    }

    void giveDamage(float damage);
private:
    float m_movementSpeed;
    float m_healthPoints;
    float m_attackDamage;
};
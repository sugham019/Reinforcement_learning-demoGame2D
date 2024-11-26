#include "Character.hpp"
#include "GameObject.hpp"


Character::Character(float healthPoints, float attackDamage, sf::Sprite& sprite, const sf::Vector2f& posInMap, bool isRigid, GameObjectType type, float movementSpeed)
: m_healthPoints(healthPoints), m_attackDamage(attackDamage), GameObject(sprite, posInMap, isRigid, type), m_movementSpeed(movementSpeed){

}

void Character::giveDamage(float damage){
    m_healthPoints -= damage * m_attackDamage;

    if(m_healthPoints < 0)
        m_healthPoints = 0;
}
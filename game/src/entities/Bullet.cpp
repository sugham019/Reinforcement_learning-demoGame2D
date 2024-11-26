#include "Bullet.hpp"
#include "../components/ErrorHandler.hpp"

Bullet::Bullet(sf::Sprite& sprite, const sf::Vector2f& posInMap, const sf::Vector2f& direction, float speed)
    : GameObject(sprite, posInMap, true, GameObjectType::BULLET), m_speed(speed), m_direction(direction){

}

void Bullet::changeSpeed(float newSpeed){
    if(newSpeed <= 0){
        reportWarning("Cannot have bullet speed less or equal to zero");
        return;
    }
    m_speed = newSpeed;
}
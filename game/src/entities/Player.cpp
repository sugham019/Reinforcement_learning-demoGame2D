#include "Player.hpp"
#include <SFML/System/Vector2.hpp>

Player::Player(sf::Sprite& sprite, const sf::Vector2f& spawnPosition, float healthPoints, float attackDamage, bool isRigid)
    : Character(healthPoints, attackDamage, sprite, spawnPosition, isRigid, GameObjectType::PLAYER, 0.2f){

}

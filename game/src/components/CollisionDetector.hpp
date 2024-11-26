#pragma once
#include <SFML/System/Vector2.hpp>

class CollisionDetector{

public:
    virtual bool isObjectColliding(const sf::Vector2f& obj1Pos, const sf::Vector2u& obj1Size, 
                const sf::Vector2f& obj2Pos, const sf::Vector2u& obj2Size) = 0;
};

class AABBCollisionDetector: public CollisionDetector{

public:
    bool isObjectColliding(const sf::Vector2f& obj1Pos, const sf::Vector2u& obj1Size, 
                const sf::Vector2f& obj2Pos, const sf::Vector2u& obj2Size) override;
    
};
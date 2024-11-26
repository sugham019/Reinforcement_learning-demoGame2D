#include "CollisionDetector.hpp"

bool AABBCollisionDetector::isObjectColliding(const sf::Vector2f& obj1Pos, const sf::Vector2u& obj1Size, 
                const sf::Vector2f& obj2Pos, const sf::Vector2u& obj2Size){

    const float xCordsRightObj1 = obj1Pos.x + obj1Size.x;
    const float xCordsRightObj2 = obj2Pos.x + obj2Size.x;

    if(obj2Pos.x >= obj1Pos.x && obj2Pos.x <= xCordsRightObj1
            || xCordsRightObj2 >= obj1Pos.x && xCordsRightObj2 <= xCordsRightObj1
            || obj1Pos.x >= obj2Pos.x && obj1Pos.x <= xCordsRightObj2
            || xCordsRightObj1 >= obj2Pos.x && xCordsRightObj1 <= xCordsRightObj2){
        
        const float yCordsDownObj1 = obj1Pos.y + obj1Size.y;
        const float yCordsDownObj2 = obj2Pos.y + obj2Size.y;

        if(obj2Pos.y >= obj1Pos.y && obj2Pos.y <= yCordsDownObj1
            || yCordsDownObj2 >= obj1Pos.y && yCordsDownObj2 <= yCordsDownObj1
            || obj1Pos.y >= obj2Pos.y && obj1Pos.y <= yCordsDownObj2
            || yCordsDownObj1 >= obj2Pos.y && yCordsDownObj1 <= yCordsDownObj2){
            
            return true;
        }
    }
    return false;
}


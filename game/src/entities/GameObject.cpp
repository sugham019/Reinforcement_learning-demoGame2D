#include "GameObject.hpp"
#include "../components/ErrorHandler.hpp"
#include <SFML/System/Vector2.hpp>
#include "../components/Map.hpp"

GameObject::GameObject(sf::Sprite& sprite, const sf::Vector2f& posInMap, bool isRigid, GameObjectType objectType)
: m_sprite(sprite), m_posInMap(posInMap), m_isRigid(isRigid), m_type(objectType){

    if(posInMap.x < 0 || posInMap.y < 0){
        reportError("Object cannot spawn at negative coordinate");
    }
    m_chunk = posInMap.x / CHUNK_WIDTH;
}

void GameObject::lockCamera(sf::Vector2f& cam){
    cam.x = m_posInMap.x;
    cam.y = m_posInMap.y;
    m_lockedCamera = &cam;
}

void GameObject::updatePosInMap(const sf::Vector2f& offset){
    m_posInMap.x += offset.x;
    m_posInMap.y += offset.y;

    if(m_posInMap.x < 0 || m_posInMap.y < 0){
        reportError("Cannot move object to negative coordinate");
    }
    
    if(m_lockedCamera != nullptr){
        m_lockedCamera->x += offset.x;
        m_lockedCamera->y += offset.y;
    }
}
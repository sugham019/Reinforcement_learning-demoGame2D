#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>

enum class GameObjectType{

    PLAYER,
    SPIKE,
    TILE,
    LAVA,
    BULLET
};

class GameObject{

public:
    GameObject(sf::Sprite& sprite, const sf::Vector2f& posInMap, bool isRigid, GameObjectType type); 
    
    void updatePosInMap(const sf::Vector2f& offset);

    const sf::Vector2f& getPosInMap() const{
        return m_posInMap;
    }

    const sf::Transform& getTransform() const{
        return m_transform;
    }

    void setSpritePosition(const sf::Vector2f& position) const{
        m_sprite.setPosition(position);
    }

    void lockCamera(sf::Vector2f& cam);

    const sf::Sprite& getSprite() const{
        return m_sprite;
    }

    const bool& isRigid(){
        return m_isRigid;
    }

    sf::Vector2u getSize() const{
        return m_sprite.getTexture()->getSize();;
    }

    bool isCamLocked() const{
        return m_lockedCamera == nullptr;
    }

    const GameObjectType& getType() const{
        return m_type;
    }
    
    int m_chunk;
    
private:
    sf::Transform m_transform;
    sf::Vector2f m_posInMap;
    sf::Vector2f* m_lockedCamera = nullptr;
    sf::Sprite& m_sprite;
    GameObjectType m_type;
    bool m_isRigid;
};

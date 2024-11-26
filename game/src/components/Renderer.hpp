#pragma once
#include "Map.hpp"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

class Renderer{

public:
    Renderer(sf::RenderWindow& gameWindow);
    void draw(const sf::Vector2f& cameraPos, const Map& map);

private:
    void drawObjectInCanva(const GameObject& gameObject, const sf::Vector2f& offsetFromCamera);
    void adjustCameraRange();
    bool isInsideCameraRange(const sf::Vector2f& objectPosition, const sf::Vector2f& cameraPos);

    sf::RenderTexture m_canvaTexture;
    sf::Sprite m_canvaSprite;
    sf::RenderWindow& m_gameWindow;
    sf::Transform m_canvaTransform;
    
    sf::Vector2f m_cameraLocationInCanva;
    int m_cameraRangeUp, m_cameraRangeDown, m_cameraRangeLeft, m_cameraRangeRight;
};
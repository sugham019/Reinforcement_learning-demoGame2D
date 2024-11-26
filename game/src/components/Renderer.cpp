#include "Renderer.hpp"
#include "Map.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>


Renderer::Renderer(sf::RenderWindow& gameWindow): m_gameWindow(gameWindow){

    m_cameraLocationInCanva = sf::Vector2f(0.2 * WINDOW_WIDTH, 0.8 * WINDOW_HEIGHT);
    adjustCameraRange();
    m_canvaTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_canvaSprite.setTexture(m_canvaTexture.getTexture());
}

void Renderer::adjustCameraRange(){
    m_cameraRangeLeft = m_cameraLocationInCanva.x;
    m_cameraRangeRight = WINDOW_WIDTH - m_cameraLocationInCanva.x;
    m_cameraRangeUp = m_cameraLocationInCanva.y;
    m_cameraRangeDown = WINDOW_HEIGHT - m_cameraLocationInCanva.y;
}

void Renderer::draw(const sf::Vector2f& cameraPos, const Map& map){

    m_canvaTexture.clear();
    auto drawChunk = [&](MapChunk& mapChunk){
        for(auto gameObject: mapChunk.gameObjects){
            const sf::Vector2f pos = gameObject->getPosInMap();
            const sf::Vector2f offsetFromCamera(pos.x - cameraPos.x, pos.y - cameraPos.y); 
            drawObjectInCanva(*gameObject, offsetFromCamera);
        }

    };

    for(auto mapChunk: map.mapChunks){
        drawChunk(mapChunk);
    }
    m_canvaTexture.display();
    m_gameWindow.draw(m_canvaSprite, m_canvaTransform);
}

void Renderer::drawObjectInCanva(const GameObject& gameObject, const sf::Vector2f& offsetFromCamera){
    sf::Vector2f pos(m_cameraLocationInCanva.x + offsetFromCamera.x, m_cameraLocationInCanva.y + offsetFromCamera.y);
    gameObject.setSpritePosition(pos);
    m_canvaTexture.draw(gameObject.getSprite(), gameObject.getTransform());
}


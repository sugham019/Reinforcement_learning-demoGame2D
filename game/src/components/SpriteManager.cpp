#include "SpriteManager.hpp"
#include "ErrorHandler.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

SpriteManager::SpriteManager(const std::string& assetsLocation){
    initializeSprites(assetsLocation);
}

SpriteManager::~SpriteManager(){
    for(sf::Texture* texture: m_textures){
        delete texture;
    }    
}

SpriteManager& SpriteManager::getInstance(){
    static SpriteManager spriteManager("assets/");
    return spriteManager;
}

void SpriteManager::initializeSprites(const std::string& assetsLocation){
    
    loadTextureIntoSprite(player, assetsLocation + "player.png");
    loadTextureIntoSprite(tile, assetsLocation + "tile.png");
    loadTextureIntoSprite(lava, assetsLocation + "lava.png");
    loadTextureIntoSprite(spike, assetsLocation + "spike.png");
    loadTextureIntoSprite(bullet, assetsLocation + "bullet.png");
}

void SpriteManager::loadTextureIntoSprite(sf::Sprite& sprite, const std::string& imgFile){
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(imgFile)) {
        reportError("Failed to load img file : "+ imgFile);
    }
    sprite.setTexture(*texture);
    m_textures.push_back(texture);
}

#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>

class SpriteManager{

public:
    static SpriteManager& getInstance();

    sf::Sprite player;
    sf::Sprite spike;
    sf::Sprite tile;
    sf::Sprite lava;
    sf::Sprite bullet;

    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;

private:
    SpriteManager(const std::string& assetsLocation);
    ~SpriteManager();

    void initializeSprites(const std::string& assetsLocation);
    void loadTextureIntoSprite(sf::Sprite& sprite, const std::string& imgFile);

    std::vector<sf::Texture*> m_textures;
};
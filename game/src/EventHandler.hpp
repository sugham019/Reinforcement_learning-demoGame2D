#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "components/Keymap.hpp"

class Game;

class EventHandler{

public:
    EventHandler(Game& game);
    void submitEvent(sf::Event& event);

private:
    void handleKeyPressedEvent(const sf::Event::KeyEvent& keyEvent);
    void handleKeyReleasedEvent(const sf::Event::KeyEvent& keyEvent);

    Game& m_game;
    Keymap& m_keymap;
};
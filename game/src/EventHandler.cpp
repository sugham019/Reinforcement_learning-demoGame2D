#include "EventHandler.hpp"
#include "Game.hpp"

EventHandler::EventHandler(Game& game): m_game(game), m_keymap(game.m_keymap){

}

void EventHandler::submitEvent(sf::Event& event){
    switch(event.type){
        case sf::Event::KeyPressed:
            handleKeyPressedEvent(event.key);
            break;
        case sf::Event::KeyReleased:
            handleKeyReleasedEvent(event.key);
            break;
        case sf::Event::Closed:
            m_game.m_window->close();
            break;
        default:
            break;
    }
}

void EventHandler::handleKeyPressedEvent(const sf::Event::KeyEvent& keyEvent){
    if(keyEvent.code == m_keymap.Resume){
        m_game.m_isPaused = false;
    }

    if(!m_game.m_isPaused){
        if(keyEvent.code == m_keymap.Jump){
            m_game.jump();
        }else if(keyEvent.code == m_keymap.Shoot){
            m_game.shoot();
        }
    }
}

void EventHandler::handleKeyReleasedEvent(const sf::Event::KeyEvent& keyEvent){

}


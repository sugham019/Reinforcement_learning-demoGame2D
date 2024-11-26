#pragma once
#include <SFML/Window/Keyboard.hpp>

using Key = sf::Keyboard::Key;

struct Keymap{

    Key Jump, Shoot;
};

constexpr Keymap getDefaultKeymap(){
    return Keymap{Key::Space, Key::F};
}
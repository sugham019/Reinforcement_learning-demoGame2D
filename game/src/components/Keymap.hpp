#pragma once
#include <SFML/Window/Keyboard.hpp>

using Key = sf::Keyboard::Key;

struct Keymap{

    Key Jump, Shoot, Resume;
};

constexpr Keymap getDefaultKeymap(){
    return Keymap{Key::Space, Key::F, Key::Enter};
}
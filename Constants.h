#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <string>
#include <SFML/Window/Keyboard.hpp>

struct Constants
{
    static const std::string LEVEL_BASE_NAME;
    static const std::string LEVEL_EXTENSION;

    static const float GAMEFIELD_HEIGHT = 30.f; //visible gamefield in m
    // width is calculated based on aspect & height

    static const sf::Keyboard::Key PHYSDEBUG_KEY = sf::Keyboard::P;
    static const sf::Keyboard::Key LEVELEDIT_KEY = sf::Keyboard::F1;
};

#endif // CONSTANTS_H_INCLUDED

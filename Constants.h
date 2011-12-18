#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <string>
#include <SFML/Window/Keyboard.hpp>

struct Constants
{
    static const std::string LEVEL_BASE_NAME;
    static const std::string LEVEL_EXTENSION;

    static const std::string FONTPATH;

    static const float GAMEFIELD_HEIGHT = 30.f; //visible gamefield in m
    // width is calculated based on aspect & height

    static const float STATICRECT_BORDERSIZE = -0.1f;

    static const sf::Keyboard::Key PHYSDEBUG_KEY = sf::Keyboard::P;
    static const sf::Keyboard::Key LEVELEDIT_KEY = sf::Keyboard::F1;
    static const sf::Keyboard::Key PREVLEVEL_KEY = sf::Keyboard::F2; // not yet implemented
    static const sf::Keyboard::Key NEXTLEVEL_KEY = sf::Keyboard::F3; // not yet implemented
    static const sf::Keyboard::Key LEVELSAVE_KEY = sf::Keyboard::F5; // F4 was a bad idea - alt+f4 = quit
};

#endif // CONSTANTS_H_INCLUDED

#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <string>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>

struct Constants
{
    static const std::string LEVEL_BASE_NAME;
    static const std::string LEVEL_EXTENSION;

    static const std::string FONTPATH;

    static const float GAMEFIELD_HEIGHT; //visible gamefield in m
    // width is calculated based on aspect & height


    static const sf::Color PLAYER_COLOR;
    static const sf::Color GOAL_COLOR;
    static const sf::Color KILL_COLOR;

    static const float STATICRECT_BORDERSIZE;
    static const float EDITMODE_CAMSPEED;
    static const unsigned int LEVELCHANGE_BLINKTIME;

    static const unsigned int EXPLOSION_NUM_PARTICLES;
    static const unsigned int EXPLOSION_PARTICLE_LIFETIME_MSEC;
    static const float EXPLOSION_PARTICLE_SPEED;

    // directly gameplay influencing
    static const float GRAVITY;
    static const float PLAYER_RADIUS;

    static const float PLAYER_DAMPING;
    static const float PLAYER_FRICTION;
    static const float PLAYER_DENSITY;
    static const float PLAYER_MOVE_FORCE;

    // input
    static const sf::Keyboard::Key PHYSDEBUG_KEY;
    static const sf::Keyboard::Key LEVELEDIT_KEY;
    static const sf::Keyboard::Key PREVLEVEL_KEY; // not yet implemented
    static const sf::Keyboard::Key NEXTLEVEL_KEY; // not yet implemented
    static const sf::Keyboard::Key LEVELSAVE_KEY; // F4 was a bad idea - alt+f4 = quit

    static const sf::Keyboard::Key MOVEL_KEY;
    static const sf::Keyboard::Key MOVER_KEY;
    static const sf::Keyboard::Key MOVEU_KEY; // a.k.a. Jump
    static const sf::Keyboard::Key MOVED_KEY;
    static const sf::Keyboard::Key JUMP_KEY; // alternative key
    static const sf::Keyboard::Key RELOAD_KEY;
};

#endif // CONSTANTS_H_INCLUDED

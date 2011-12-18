#include "Constants.h"

const std::string Constants::LEVEL_BASE_NAME = "data/levels/lvl";
const std::string Constants::LEVEL_EXTENSION = ".txt";
const std::string Constants::FONTPATH = "data/fonts/chary___.ttf";


const float Constants::GAMEFIELD_HEIGHT = 30.f; //visible gamefield in m
// width is calculated based on aspect & height

const float Constants::STATICRECT_BORDERSIZE = -0.1f;
const float Constants::EDITMODE_CAMSPEED = 20.f/1000.f;
const sf::Color Constants::PLAYER_COLOR(63, 63, 255);
const sf::Color Constants::GOAL_COLOR(0, 255, 0, 127);
const unsigned int Constants::LEVELCHANGE_BLINKTIME = 250;

//  Gameplay affecting

const float Constants::GRAVITY = 15.f;
const float Constants::PLAYER_HALFHEIGHT = 0.9f; //let the player be 1.8m high
const float Constants::PLAYER_HALFWIDTH = 0.3f; // and 60cm wide
const float Constants::PLAYER_DAMPING = 0.0f;
const float Constants::PLAYER_FRICTION = 0.1f;
const float Constants::PLAYER_DENSITY = 5.f;
const float Constants::PLAYER_MAX_VELOCITY = 15.0f; //fastest speed /through player input/
const float Constants::PLAYER_MOVE_FORCE = 80.f;
const float Constants::PLAYER_JUMP_FORCE = 50.f;

//  Input

const sf::Keyboard::Key Constants::PHYSDEBUG_KEY = sf::Keyboard::P;
const sf::Keyboard::Key Constants::LEVELEDIT_KEY = sf::Keyboard::F1;
const sf::Keyboard::Key Constants::PREVLEVEL_KEY = sf::Keyboard::F2;
const sf::Keyboard::Key Constants::NEXTLEVEL_KEY = sf::Keyboard::F3;
const sf::Keyboard::Key Constants::LEVELSAVE_KEY = sf::Keyboard::F5; // F4 was a bad idea - alt+f4 = quit

const sf::Keyboard::Key Constants::MOVEL_KEY = sf::Keyboard::A;
const sf::Keyboard::Key Constants::MOVER_KEY = sf::Keyboard::D;
const sf::Keyboard::Key Constants::MOVEU_KEY = sf::Keyboard::W;
const sf::Keyboard::Key Constants::MOVED_KEY = sf::Keyboard::S;
const sf::Keyboard::Key Constants::JUMP_KEY = sf::Keyboard::Space;
const sf::Keyboard::Key Constants::RELOAD_KEY = sf::Keyboard::R;

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
const sf::Color Constants::KILL_COLOR(255, 0, 0);
const unsigned int Constants::LEVELCHANGE_BLINKTIME = 250; // deprecated

const unsigned int Constants::EXPLOSION_NUM_PARTICLES = 128;
const unsigned int Constants::EXPLOSION_PARTICLE_LIFETIME_MSEC = 3000;
const float Constants::EXPLOSION_PARTICLE_SPEED = 5.0f / 1000.f; //random direction

//  Gameplay affecting

const float Constants::GRAVITY = 0.f;

const float Constants::PLAYER_RADIUS = 0.5f;

const float Constants::PLAYER_DAMPING = 0.0f;
const float Constants::PLAYER_FRICTION = 0.1f;
const float Constants::PLAYER_DENSITY = 5.f;

const float Constants::PLAYER_MOVE_FORCE = 80.f;

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

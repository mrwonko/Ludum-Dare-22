#ifndef SOUNDS_H
#define SOUNDS_H

#include <SFML/Audio/SoundBuffer.hpp>

struct Sounds
{
    sf::SoundBuffer Death;
    sf::SoundBuffer Jump;
    sf::SoundBuffer Start;

    const bool Load()
    {
        return Death.loadFromFile("data/sounds/death.wav") &&
               Jump.loadFromFile("data/sounds/jump.wav") &&
               Start.loadFromFile("data/sounds/levelstart.wav");
    }
};

#endif // SOUNDS_H

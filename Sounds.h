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
        return Death.LoadFromFile("data/sounds/death.wav") &&
               Jump.LoadFromFile("data/sounds/jump.wav") &&
               Start.LoadFromFile("data/sounds/levelstart.wav");
    }
};

#endif // SOUNDS_H

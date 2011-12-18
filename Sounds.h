#ifndef SOUNDS_H
#define SOUNDS_H

#include <SFML/Audio/SoundBuffer.hpp>

struct Sounds
{
    sf::SoundBuffer Death;

    const bool Load()
    {
        return Death.LoadFromFile("data/sounds/death.wav");
    }
};

#endif // SOUNDS_H

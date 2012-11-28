#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics/Texture.hpp>

struct Textures
{
    sf::Texture Spark;
    sf::Texture Explosion;

    const bool Load()
    {
        return Spark.loadFromFile("data/textures/spark.png") &&
               Explosion.loadFromFile("data/textures/explosion.png");
    }
};

#endif // TEXTURES_H

#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics/Texture.hpp>

struct Textures
{
    sf::Texture Spark;
    sf::Texture Explosion;

    const bool Load()
    {
        return Spark.LoadFromFile("data/textures/spark.png") &&
               Explosion.LoadFromFile("data/textures/explosion.png");
    }
};

#endif // TEXTURES_H

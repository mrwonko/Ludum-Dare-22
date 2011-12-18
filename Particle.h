#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics/Sprite.hpp>


class Particle : public sf::Sprite
{
    public:
        sf::Vector2f Velocity;
        int RemainingLife_msec;
        int TotalLifetime_msec; //Opacity is RemainingLife/TotalLifetime

        Particle() {}
        virtual ~Particle() {}
    protected:
    private:
};

#endif // PARTICLE_H

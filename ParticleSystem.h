#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <SFML/Graphics/Drawable.hpp>
#include <list>

class Particle;

class ParticleSystem : public sf::Drawable
{
    public:
        ParticleSystem();
        virtual ~ParticleSystem();

        void CreateExplosion(const sf::Vector2f& pos);
        void CreateMoveSpark(const sf::Vector2f& pos, const float sizeMult);

        void Update(unsigned int deltaT_msec);

        void Clear();
    private:
		virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
        typedef std::list<Particle*> ParticleList;
        ParticleList mParticles;
};

#endif // PARTICLESYSTEM_H

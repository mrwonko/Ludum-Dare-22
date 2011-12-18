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
        void CreateMovespark(const sf::Vector2f& pos);

        void Update(unsigned int deltaT_msec);

        void Clear();
    private:
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
        typedef std::list<Particle*> ParticleList;
        ParticleList mParticles;
};

#endif // PARTICLESYSTEM_H

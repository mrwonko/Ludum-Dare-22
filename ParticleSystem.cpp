#include "ParticleSystem.h"
#include "Particle.h"
#include "Constants.h"
#include "Textures.h"
#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>

extern Textures g_Textures;


ParticleSystem::ParticleSystem()
{
    //ctor
}

ParticleSystem::~ParticleSystem()
{
    Clear();
}

void ParticleSystem::Clear()
{
    for(ParticleList::iterator it = mParticles.begin(); it != mParticles.end(); ++it)
    {
        delete *it;
    }
    mParticles.clear();
}

void ParticleSystem::Update(unsigned int deltaT_msec)
{
    for(ParticleList::iterator it = mParticles.begin(); it != mParticles.end(); /*nothing*/)
    {
        Particle* p = *it;
        p->RemainingLife_msec -= deltaT_msec;
        if(p->RemainingLife_msec <= 0)
        {
            it = mParticles.erase(it);
            delete p;
        }
        else
        {
            float alpha = 255.f*p->RemainingLife_msec / p->TotalLifetime_msec;
            p->setColor(sf::Color(alpha, alpha, alpha)); //since I use Blendmode Add
            sf::Vector2f newPos(p->setPosition());
            newPos += float(deltaT_msec) * (p->Velocity);
            p->setPosition(newPos);
            ++it;
        }
    }
}

void ParticleSystem::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    for(ParticleList::const_iterator it = mParticles.begin(); it != mParticles.end(); ++it)
    {
        target.Draw(**it);
    }
}

void ParticleSystem::CreateExplosion(const sf::Vector2f& pos)
{
    Particle pattern;
    pattern.setTexture(g_Textures.Explosion, true);
    pattern.setScale(0.03f, 0.03f);
    pattern.setBlendMode(sf::BlendAdd);
    pattern.TotalLifetime_msec = pattern.RemainingLife_msec = Constants::EXPLOSION_PARTICLE_LIFETIME_MSEC;
    pattern.setPosition(pos);
    for(unsigned int i = 0; i < Constants::EXPLOSION_NUM_PARTICLES; ++i)
    {
        Particle* p = new Particle(pattern);
        float movedir = (float(i) / float(Constants::EXPLOSION_NUM_PARTICLES)) * M_PI * 2.f;
        p->Velocity = Constants::EXPLOSION_PARTICLE_SPEED * sf::Vector2f(cos(movedir), sin(movedir));
        p->SetRotation(movedir / M_PI * 180.f);
        mParticles.push_back(p);
    }
}

void ParticleSystem::CreateShot(const sf::Vector2f& startPos, const sf::Vector2f& endPos)
{
    sf::Vector2f dif = endPos - startPos;

    Particle* p = new Particle;
    p->setTexture(g_Textures.Explosion, true);
    p->setScaleY(.03f);
    p->setScaleX(sqrtf(dif.x * dif.x + dif.y * dif.y) / g_Textures.Explosion.GetWidth());
    p->TotalLifetime_msec = p->RemainingLife_msec = 1000;
    p->setPosition(startPos);
    p->setRotation(atan2(dif.y, dif.x) * 180.f / M_PI);
    p->setBlendMode(sf::BlendAdd);
    mParticles.push_back(p);
}

#include "ParticleSystem.h"
#include "Particle.h"
#include "Constants.h"
#include "Textures.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>

extern Textures g_Textures;

static boost::mt19937 s_rng;

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
            p->SetColor(sf::Color(alpha, alpha, alpha)); //since I use Blendmode Add
            sf::Vector2f newPos(p->GetPosition());
            newPos += float(deltaT_msec) * (p->Velocity);
            p->SetPosition(newPos);
            ++it;
        }
    }
}

#include <iostream>
void ParticleSystem::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    for(ParticleList::const_iterator it = mParticles.begin(); it != mParticles.end(); ++it)
    {
        target.Draw(**it);
    }
}

void ParticleSystem::CreateExplosion(const sf::Vector2f& pos)
{
    std::cout<<"Create Explosion..."<<std::endl;
    Particle pattern;
    boost::uniform_01<boost::mt19937> FloatGenerator(s_rng);
    pattern.SetTexture(g_Textures.Explosion, true);
    pattern.SetScale(0.03f, 0.03f);
    pattern.SetBlendMode(sf::Blend::Add);
    pattern.TotalLifetime_msec = pattern.RemainingLife_msec = Constants::EXPLOSION_PARTICLE_LIFETIME_MSEC;
    pattern.SetPosition(pos);
    for(unsigned int i = 0; i < Constants::EXPLOSION_NUM_PARTICLES; ++i)
    {
        Particle* p = new Particle(pattern);
        float movedir = FloatGenerator() * M_PI * 2.f;
        p->Velocity = Constants::EXPLOSION_PARTICLE_SPEED * sf::Vector2f(cos(movedir), sin(movedir));
        p->SetRotation(movedir / M_PI * 180.f);
        mParticles.push_back(p);
    }
    std::cout<<"...done!"<<std::endl;
}

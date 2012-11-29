#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include <SFML/Audio/Sound.hpp>

class b2Body;

class Player : public Object
{
    public:
        Player(Level* const level);
        virtual ~Player();

        virtual const std::string GetType() const { return "Player"; }

        virtual const bool Serialize(std::ostream& out_stream) const;
        virtual const bool Deserialize(std::istream& stream);

        /** \return Whether the event was used **/
        const bool ProcessEvent(const sf::Event& event);

        virtual void Update(unsigned int deltaT_msec);

        void SetPosition(const sf::Vector2f& pos);

        void Stop();

    private:
		void setPosition(); //renamed in SFML to this - not implementing this so I get an error if I accidentally call it
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
        b2Body* mBody;
        sf::Shape mRepresentative;
        bool mJumpedThisFrame;
        sf::Sound mJumpChannel;
        float mNextMoveSparkTime; //time until next movespark is spawned
};

#endif // PLAYER_H

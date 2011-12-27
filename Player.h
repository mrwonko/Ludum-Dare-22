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
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
        b2Body* mBody;
        sf::Shape mRepresentative;
        int mIndex; //which player? (default: -1 = invalid)
};

#endif // PLAYER_H

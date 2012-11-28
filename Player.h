#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include <SFML/Audio/Sound.hpp>

class b2Body;
namespace sf
{
    class Packet;
}

class Player : public Object
{
    public:
        Player(Level* const level);
        virtual ~Player();

        virtual const std::string GetType() const { return "Player"; }

        virtual const bool Serialize(std::ostream& out_stream) const;
        virtual const bool Serialize(sf::Packet& out_packet) const;
        virtual const bool Deserialize(sf::Packet& packet);
        virtual const bool Deserialize(std::istream& stream);

        const bool SerializeOrders(sf::Packet& out_packet) const;
        const bool DeserializeOrders(sf::Packet& packet);

        /** \return Whether the event was used **/
        const bool ProcessEvent(const sf::Event& event);

        virtual void Update(unsigned int deltaT_msec);

        void SetPosition(const sf::Vector2f& pos);

        void Stop();

        const bool IsDead() const { return mDead; }

        void Kill();

        const int GetIndex() const { return mIndex; }
        void SetIndex(const int index) { mIndex = index; UpdateIndex(); }

    private:
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
        b2Body* mBody;
        sf::Shape mRepresentative;
        int mIndex; //which player? (default: -1 = invalid)
        sf::Text mIndexText;
        bool mDead;

        sf::Shape mAngleIndicator; //not only indicates the angle, also stores it (its rotation)

        void UpdateIndex();

        int mTargetIndex; //-1 = none
        int mAimTime; ///< for how long have I aimed so far?

        const bool IsVisible(Player* player);
};

#endif // PLAYER_H

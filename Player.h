#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

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

    private:
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
};

#endif // PLAYER_H

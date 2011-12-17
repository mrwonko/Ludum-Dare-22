#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Player : public Object
{
    public:
        Player();
        virtual ~Player();

        virtual const bool Serialize(std::ostream& out_stream) const;
        virtual const bool Deserialize(std::istream& stream);
        virtual const std::string GetType() const
        {
            return "player";
        }

        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;

    private:
};

#endif // PLAYER_H

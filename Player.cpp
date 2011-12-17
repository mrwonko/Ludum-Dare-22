#include "Player.h"

Player::Player(Level* const level):
    Object(level)
{
    //ctor
}

Player::~Player()
{
    //dtor
}

void Player::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    //TODO: player rendering
}

const bool Player::Serialize(std::ostream& out_stream) const
{
    //TODO: player serialization
    return true;
}
const bool Player::Deserialize(std::istream& stream)
{
    //TODO: player deserialization
    return true;
}

#include "StaticRect.h"

StaticRect::StaticRect(Level* const level) :
    Object(level)
{
    //ctor
}

StaticRect::~StaticRect()
{
    //dtor
}

const bool StaticRect::Serialize(std::ostream& out_stream) const
{
    return true;
}

const bool StaticRect::Deserialize(std::istream& stream)
{
    return true;
}

void StaticRect::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    //foo
}

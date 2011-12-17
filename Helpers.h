#ifndef HELPERS_H
#define HELPERS_H

namespace sf
{
    template <typename T> class Vector2;
    typedef Vector2<float> Vector2f;
}

// height defined in constants.h, width calculated based on aspect
void SetViewPos(const sf::Vector2f& center);

#endif

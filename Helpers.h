#ifndef HELPERS_H
#define HELPERS_H

#include <string>

namespace sf
{
    template <typename T> class Vector2;
    typedef Vector2<float> Vector2f;
}

// height defined in constants.h, width calculated based on aspect
void SetViewPos(sf::RenderWindow& window, const sf::Vector2f& center);

unsigned int GetLevelCount();

const std::string GetLevelName(unsigned int index);

#endif

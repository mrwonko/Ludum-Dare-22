#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>

namespace sf
{
    template <typename T> class Vector2;
    typedef Vector2<float> Vector2f;
    class RenderWindow;
}

// height defined in constants.h, width calculated based on aspect
void SetViewPos(sf::RenderWindow& window, const sf::Vector2f& center);

std::string AskForMapName();

// uses g_Window - yep, I start quick & dirty hacking.
sf::Vector2f ScreenToWorldSpace(const sf::Vector2f& screenSpaceCoord);
sf::Vector2f ProcessEditMousePos(const sf::Vector2f& pos);


class b2World;
class b2Body;
class b2Vec2;

std::vector<b2Body*> GetBodiesAtPoint(const b2World& world, const b2Vec2& point);

#endif

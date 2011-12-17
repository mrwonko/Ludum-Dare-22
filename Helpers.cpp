#include <SFML/Graphics.hpp>
#include "Helpers.h"
#include "Constants.h"
#include <fstream>
#include <sstream>
#include "Box2D/Box2D.h"

void SetViewPos(sf::RenderWindow& window, const sf::Vector2f& center)
{
    //I could cache this by making it static but the viewsize might change?
    sf::Vector2f viewSize(float(window.GetWidth()) / window.GetHeight() * Constants::GAMEFIELD_HEIGHT, Constants::GAMEFIELD_HEIGHT);
    sf::View view(center, viewSize);
    window.SetView(view);
}

unsigned int GetLevelCount()
{
    unsigned int count = 0;
    while(true)
    {
        std::ifstream file(GetLevelName(count).c_str());
        if(file.fail())
        {
            return count;
        }
        ++count;
    }
}

const std::string GetLevelName(unsigned int index)
{
    std::stringstream levelname;
    levelname << Constants::LEVEL_BASE_NAME << index << Constants::LEVEL_EXTENSION;
    return levelname.str();
}

namespace
{
    struct AABBQueryCallback : public b2QueryCallback
    {
        AABBQueryCallback(const b2Vec2& point) : mPoint (point) {}

        virtual bool ReportFixture (b2Fixture *fixture)
        {
            b2Body* body = fixture->GetBody();
            if(body == NULL) //skip
            {
                return true;
            }
            if(fixture->GetShape()->TestPoint(body->GetTransform(), mPoint))
            {
                foundBodies.push_back(body);
            }
            return true; //continue
        }

        std::vector<b2Body*> foundBodies;
    private:
        const b2Vec2& mPoint;
    };
}

std::vector<b2Body*> GetBodiesAtPoint(const b2World* world, const b2Vec2& point)
{
    static const b2Vec2 tolerance(0.01f, 0.01f); //there's actually zero tolerance - we test whether the found fixtures' shapes contain the searched points later.
    b2AABB aabb;
    aabb.upperBound = point + tolerance;
    aabb.lowerBound = point - tolerance;
    AABBQueryCallback cb(point);
    world->QueryAABB(&cb, aabb);
    return cb.foundBodies;
}

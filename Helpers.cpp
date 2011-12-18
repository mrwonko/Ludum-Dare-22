#include <SFML/Graphics.hpp>
#include "Helpers.h"
#include "Constants.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <Box2D/Box2D.h>

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
                std::cout<<"Warning: AABBQuery found fixture without body!" << std::endl;
                return true;
            }
            if(fixture->TestPoint(mPoint))
            {
                foundBodies.push_back(body);
            }
            #ifdef _DEBUG
            else
            {
                std::cout<<"AABB too inaccurate - body not exactly hit. " << mPoint.x << ", " << mPoint.y << "; " << body->GetTransform().p.x << ", " << body->GetTransform().p.y << std::endl;
            }
            #endif
            return true; //continue
        }

        std::vector<b2Body*> foundBodies;
    private:
        const b2Vec2& mPoint;
    };
}

std::vector<b2Body*> GetBodiesAtPoint(const b2World& world, const b2Vec2& point)
{
    static const b2Vec2 tolerance(0.01f, 0.01f); //there's actually zero tolerance - we test whether the found fixtures' shapes contain the searched points later.
    b2AABB aabb;
    aabb.upperBound = point + tolerance;
    aabb.lowerBound = point - tolerance;
    AABBQueryCallback cb(point);
    world.QueryAABB(&cb, aabb);
    return cb.foundBodies;
}

extern sf::RenderWindow* g_Window;
sf::Vector2f ScreenToWorldSpace(const sf::Vector2f& screenSpaceCoord)
{
    assert(g_Window != NULL);
    const sf::Vector2f& viewSize = g_Window->GetView().GetSize();
    const sf::Vector2f viewCorner = g_Window->GetView().GetCenter() - 0.5f * viewSize;
    return sf::Vector2f(viewCorner.x + screenSpaceCoord.x * viewSize.x / g_Window->GetWidth(), viewCorner.y + screenSpaceCoord.y * viewSize.y / g_Window->GetHeight());
}

sf::Vector2f ProcessEditMousePos(const sf::Vector2f& pos)
{
    sf::Vector2f mousePos = ScreenToWorldSpace(pos);
    if(sf::Keyboard::IsKeyPressed(sf::Keyboard::LControl))
    {
        mousePos.x = int(mousePos.x + 0.5f);
        mousePos.y = int(mousePos.y + 0.5f);
    }
    return mousePos;
}

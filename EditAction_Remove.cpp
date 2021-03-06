#include "EditAction_Remove.h"
#include "Helpers.h"
#include "Level.h"
#include <Box2D/Box2D.h>
#include <SFML/Window/Event.hpp>

EditAction_Remove::EditAction_Remove(Level* const level) :
    EditAction(level)
{
    //ctor
}

EditAction_Remove::~EditAction_Remove()
{
    //dtor
}

const bool EditAction_Remove::ProcessEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        mousePos = ScreenToWorldSpace(mousePos);
        std::vector<b2Body*> bodies = GetBodiesAtPoint(mLevel->GetWorld(), b2Vec2(mousePos.x, mousePos.y));
        for(std::vector<b2Body*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        {
            Object* obj = reinterpret_cast<Object*>((*it)->GetUserData());
            if(obj->GetType() != "Player") //don't want to remove the player
            {
                mLevel->RemoveObject(obj);
                delete obj;
            }
        }
        return true;
    }
    return false;
}

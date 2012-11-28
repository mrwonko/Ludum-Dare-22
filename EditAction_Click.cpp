#include "EditAction_Click.h"
#include <SFML/Window/Event.hpp>
#include "Helpers.h"
#include <Box2D/Box2D.h>
#include "Level.h"

EditAction_Click::EditAction_Click(Level* const level) :
    EditAction(level)
{
    //ctor
}

EditAction_Click::~EditAction_Click()
{
    //dtor
}

const bool EditAction_Click::ProcessEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        mousePos = ScreenToWorldSpace(mousePos);
        std::vector<b2Body*> bodies = GetBodiesAtPoint(mLevel->GetWorld(), b2Vec2(mousePos.x, mousePos.y));
        for(std::vector<b2Body*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        {
            Object* obj = reinterpret_cast<Object*>((*it)->GetUserData());
            obj->Edit_OnClicked(event.mouseButton.button);
        }
        return true;
    }
    return false;
}

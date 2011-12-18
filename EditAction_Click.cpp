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
    if(event.Type == sf::Event::MouseButtonPressed)
    {
        std::vector<b2Body*> bodies = GetBodiesAtPoint(mLevel->GetWorld(), b2Vec2(event.MouseButton.X, event.MouseButton.Y));
        for(std::vector<b2Body*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        {
            Object* obj = reinterpret_cast<Object*>((*it)->GetUserData());
            obj->Edit_OnClicked(event.MouseButton.Button);
        }
        return true;
    }
    return false;
}

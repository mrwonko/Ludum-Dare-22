#include "EditAction_NewStaticRect.h"
#include "StaticRect.h"
#include "Level.h"
#include "Helpers.h"
#include <algorithm>

EditAction_NewStaticRect::EditAction_NewStaticRect(Level* const level) :
    EditAction(level),
    mNewRect(NULL)
{
    //ctor
}

EditAction_NewStaticRect::~EditAction_NewStaticRect()
{
    //dtor
}

const bool EditAction_NewStaticRect::ProcessEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        //starting/finishing object
        if(event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            mousePos = ProcessEditMousePos(sf::Vector2f(mousePos));
            if(mNewRect)
            {
                mNewRect->SetCorner2(mousePos);
                mNewRect = NULL;
            }
            else
            {
                mNewRect = NewStaticRect();
                mNewRect->SetCorner1(mousePos);
                mNewRect->SetCorner2(mousePos);
                mLevel->AddObject(mNewRect);
            }
        }
        //aborting
        else if(event.mouseButton.button == sf::Mouse::Right && mNewRect)
        {
            mLevel->RemoveObject(mNewRect);
            delete mNewRect;
            mNewRect = NULL;
        }
    }
    // Rect creation - size change
    else if(event.type == sf::Event::MouseMoved && mNewRect != NULL)
    {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        mNewRect->SetCorner2(ProcessEditMousePos(sf::Vector2f(mousePos)));
    }
    return false;
}


void EditAction_NewStaticRect::OnExit()
{
    if(mNewRect)
    {
        mLevel->RemoveObject(mNewRect);
        delete mNewRect;
        mNewRect = NULL;
    }
}

StaticRect* EditAction_NewStaticRect::NewStaticRect() const
{
    return new StaticRect(mLevel);
}

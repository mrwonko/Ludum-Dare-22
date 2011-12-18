#include "EditAction_NewStaticRect.h"
#include "StaticRect.h"
#include "Level.h"
#include "Helpers.h"

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
    if(event.Type == sf::Event::MouseButtonPressed)
    {
        //starting/finishing object
        if(event.MouseButton.Button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(event.MouseButton.X, event.MouseButton.Y);
            mousePos = ScreenToWorldSpace(mousePos);
            if(mNewRect)
            {
                mNewRect->SetCorner2(mousePos);
                mNewRect = NULL;
            }
            else
            {
                mNewRect = new StaticRect(mLevel);
                mNewRect->SetCorner1(mousePos);
                mNewRect->SetCorner2(mousePos);
                mLevel->AddObject(mNewRect);
            }
        }
        //aborting
        else if(event.MouseButton.Button == sf::Mouse::Right && mNewRect)
        {
            mLevel->RemoveObject(mNewRect);
            delete mNewRect;
            mNewRect = NULL;
        }
    }
    // Rect creation - size change
    else if(event.Type == sf::Event::MouseMoved && mNewRect != NULL)
    {
        sf::Vector2f mousePos(event.MouseMove.X, event.MouseMove.Y);
        mousePos = ScreenToWorldSpace(mousePos);
        mNewRect->SetCorner2(mousePos);
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

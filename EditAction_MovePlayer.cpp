#include "EditAction_MovePlayer.h"
#include "Helpers.h"
#include "Level.h"
#include <SFML/Window/Event.hpp>

EditAction_MovePlayer::EditAction_MovePlayer( Level * const level) :
    EditAction(level)
{
    //ctor
}

EditAction_MovePlayer::~EditAction_MovePlayer()
{
    //dtor
}

const bool EditAction_MovePlayer::ProcessEvent(const sf::Event& event)
{
    if(event.Type == sf::Event::MouseButtonPressed && event.MouseButton.Button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(event.MouseButton.X, event.MouseButton.Y);
        mousePos = ProcessEditMousePos(mousePos);
        mLevel->SetPlayerPosition(mousePos);
    }
    return false;
}

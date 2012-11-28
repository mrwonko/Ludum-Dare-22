#include "EditAction_NewPlayer.h"
#include "Level.h"
#include "Player.h"
#include "Helpers.h"

EditAction_NewPlayer::EditAction_NewPlayer(Level* const level) :
    EditAction(level)
{
    //ctor
}

EditAction_NewPlayer::~EditAction_NewPlayer()
{
    //dtor
}

const bool EditAction_NewPlayer::ProcessEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        mousePos = ProcessEditMousePos(sf::Vector2f(mousePos));
        Player* player = new Player(mLevel);
        player->SetPosition(mousePos);
        mLevel->AddPlayer(player);
        return true;
    }
    return false;
}
#include "EditAction_MovePlayer.h"
#include "Helpers.h"
#include "Level.h"
#include <SFML/Window/Event.hpp>

EditAction_MovePlayer::EditAction_MovePlayer( Level * const level) :
    EditAction(level),
    mPlayerIndex(0)
{
    //ctor
}

EditAction_MovePlayer::~EditAction_MovePlayer()
{
    //dtor
}

const bool EditAction_MovePlayer::ProcessEvent(const sf::Event& event)
{
    if(event.Type == sf::Event::MouseButtonPressed)
    {
        if(event.MouseButton.Button == sf::Mouse::Left) //move
        {
            sf::Vector2f mousePos(event.MouseButton.X, event.MouseButton.Y);
            mousePos = ProcessEditMousePos(mousePos);
            Player* player = mLevel->GetPlayer(mPlayerIndex);
            if(player != NULL)
            {
                player->SetPosition(mousePos);
            }
            return true;
        }
        else if(event.MouseButton.Button == sf::Mouse::Right) //select
        {
            sf::Vector2f mousePos(event.MouseButton.X, event.MouseButton.Y);
            mousePos = ProcessEditMousePos(mousePos);

            Player* player = mLevel->GetPlayer(0);
            if(player == NULL)
            {
                return true;
            }
            sf::Vector2f dif = (mousePos - player->GetPosition());
            float closestDist = dif.x * dif.x + dif.y * dif.y;
            mPlayerIndex = 0;
            for(unsigned int i = 1; i < mLevel->GetNumPlayers(); ++i)
            {
                Player* player = mLevel->GetPlayer(i);
                assert(player != NULL); //GetNumPlayers() should return the right result
                sf::Vector2f dif = (mousePos - player->GetPosition());
                float dist = dif.x * dif.x + dif.y * dif.y;
                if(dist < closestDist)
                {
                    closestDist = dist;
                    mPlayerIndex = i;
                }
            }
            return true;
        }
    }
    return false;
}

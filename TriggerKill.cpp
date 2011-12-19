#include "TriggerKill.h"
#include "Level.h"

TriggerKill::TriggerKill(Level* const level) :
    Trigger(level)
{
    StaticRect::SetColor(Constants::KILL_COLOR);
    mIsSensor = false; //objects can collide!
}

TriggerKill::~TriggerKill()
{
    //dtor
}

void TriggerKill::OnEnter()
{
    mLevel->Lose();
}

void TriggerKill::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    target.Draw(mShape);
}

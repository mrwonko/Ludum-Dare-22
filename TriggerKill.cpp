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

void TriggerKill::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
    target.draw(mShape, states);
}

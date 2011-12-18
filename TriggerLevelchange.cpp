#include "TriggerLevelchange.h"
#include "Level.h"

TriggerLevelchange::TriggerLevelchange(Level * const level) :
    Trigger(level),
    mBlinkTime(0)
{
    StaticRect::SetColor(Constants::GOAL_COLOR);
    //ctor
}

TriggerLevelchange::~TriggerLevelchange()
{
    //dtor
}

void TriggerLevelchange::OnEnter()
{
    mLevel->Win();
}

void TriggerLevelchange::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    //blinking looks ugly
    //if(mBlinkTime < Constants::LEVELCHANGE_BLINKTIME || mLevel->InEditMode())
    {
        target.Draw(mShape);
    }
}

void TriggerLevelchange::Update(unsigned int deltaT_msec)
{
    Trigger::Update(deltaT_msec);
    mBlinkTime = (mBlinkTime + deltaT_msec) % (2 * Constants::LEVELCHANGE_BLINKTIME);
}

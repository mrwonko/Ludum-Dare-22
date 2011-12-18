#ifndef TRIGGERLEVELCHANGE_H
#define TRIGGERLEVELCHANGE_H

#include "Trigger.h"


class TriggerLevelchange : public Trigger
{
    public:
        TriggerLevelchange(Level * const level);
        virtual ~TriggerLevelchange();
        virtual const std::string GetType() const { return "TriggerLevelchange"; }
        virtual void Update(unsigned int deltaT_msec);
    protected:
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
        virtual void OnEnter();
        unsigned int mBlinkTime;
};

#endif // TRIGGERLEVELCHANGE_H

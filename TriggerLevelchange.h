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
        virtual void OnEnter();
        unsigned int mBlinkTime;
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // TRIGGERLEVELCHANGE_H

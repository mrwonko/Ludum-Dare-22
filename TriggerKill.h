#ifndef TRIGGERKILL_H
#define TRIGGERKILL_H

#include "Trigger.h"


class TriggerKill : public Trigger
{
    public:
        TriggerKill(Level* const level);
        virtual ~TriggerKill();
        virtual const std::string GetType() const { return "TriggerKill"; }
    protected:
        virtual void OnEnter();
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // TRIGGERKILL_H

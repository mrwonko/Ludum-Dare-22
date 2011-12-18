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
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
};

#endif // TRIGGERKILL_H

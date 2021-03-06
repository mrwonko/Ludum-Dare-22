#ifndef TRIGGER_H
#define TRIGGER_H

#include "StaticRect.h"


class Trigger : public StaticRect
{
    public:
        virtual ~Trigger();

        virtual const std::string GetType() const { return "Trigger"; }
        virtual void Edit_OnClicked(const sf::Mouse::Button button) {}

    protected:
        virtual void OnEnter() {}
        virtual void OnLeave() {}

        Trigger(Level* const level);
        virtual void Update(unsigned int deltaT_msec);
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
        virtual void UpdateShape();

        bool mIsSensor;

    private:
        bool mPlayerWasInside;
};

#endif // TRIGGER_H

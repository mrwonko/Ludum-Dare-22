#ifndef EDITACTION_NEWSTATICRECT_H
#define EDITACTION_NEWSTATICRECT_H

#include "EditAction.h"

class StaticRect;

class EditAction_NewStaticRect : public EditAction
{
    public:
        EditAction_NewStaticRect(Level* const level);
        virtual ~EditAction_NewStaticRect();

        virtual const bool ProcessEvent(const sf::Event& event);
        virtual void OnExit();
        virtual std::string GetName() const
        {
            return "New Static Rectangle";
        }

    private:
        StaticRect* mNewRect;
};

#endif // EDITACTION_NEWSTATICRECT_H

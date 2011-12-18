#ifndef EDITACTION_REMOVE_H
#define EDITACTION_REMOVE_H

#include "EditAction.h"


class EditAction_Remove : public EditAction
{
    public:
        EditAction_Remove(Level* const level);
        virtual ~EditAction_Remove();

        virtual const bool ProcessEvent(const sf::Event& event);
        virtual std::string GetName() const
        {
            return "Delete";
        }
    protected:
    private:
};

#endif // EDITACTION_REMOVE_H

#ifndef EDITACTION_CLICK_H
#define EDITACTION_CLICK_H

#include "EditAction.h"

/// Edit Action for calling Edit_OnClicked() of the object(s) under the cursor
class EditAction_Click : public EditAction
{
    public:
        EditAction_Click(Level* const level);
        virtual ~EditAction_Click();

        virtual const bool ProcessEvent(const sf::Event& event);
        virtual std::string GetName() const
        {
            return "Click stuff";
        }
    protected:
    private:
};

#endif // EDITACTION_CLICK_H

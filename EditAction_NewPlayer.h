#ifndef EDITACTION_NEWPLAYER_H
#define EDITACTION_NEWPLAYER_H

#include "EditAction.h"


class EditAction_NewPlayer : public EditAction
{
    public:
        EditAction_NewPlayer(Level* const level);
        virtual ~EditAction_NewPlayer();

        virtual const bool ProcessEvent(const sf::Event& event);
        virtual std::string GetName() const
        {
            return "New Player";
        }
    protected:
    private:
};

#endif // EDITACTION_NEWPLAYER_H

#ifndef EDITACTION_MOVEPLAYER_H
#define EDITACTION_MOVEPLAYER_H

#include "EditAction.h"


class EditAction_MovePlayer : public EditAction
{
    public:
        EditAction_MovePlayer(Level * const level);
        virtual ~EditAction_MovePlayer();

        virtual const bool ProcessEvent(const sf::Event& event);
        virtual std::string GetName() const { return "Move Player"; }

    private:
        unsigned int mPlayerIndex;
};

#endif // EDITACTION_MOVEPLAYER_H

#ifndef EDITACTION_NEWLEVELCHANGE_H
#define EDITACTION_NEWLEVELCHANGE_H

#include "EditAction_NewStaticRect.h"


class EditAction_NewLevelchange : public EditAction_NewStaticRect
{
    public:
        EditAction_NewLevelchange(Level * const level);
        virtual ~EditAction_NewLevelchange();
        virtual std::string GetName() const
        {
            return "New Levelchange Trigger";
        }
    protected:
        virtual StaticRect* NewStaticRect() const;
};

#endif // EDITACTION_NEWLEVELCHANGE_H

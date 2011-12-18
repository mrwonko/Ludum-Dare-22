#ifndef EDITACTION_NEWKILLER_H
#define EDITACTION_NEWKILLER_H

#include "EditAction_NewStaticRect.h"


class EditAction_NewKiller : public EditAction_NewStaticRect
{
    public:
        EditAction_NewKiller(Level * const level);
        virtual ~EditAction_NewKiller();
        virtual std::string GetName() const
        {
            return "New Kill Trigger";
        }
    protected:
        virtual StaticRect* NewStaticRect() const;
};

#endif // EDITACTION_NEWKILLER_H

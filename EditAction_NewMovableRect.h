#ifndef EDITACTION_NEWMOVABLERECT_H
#define EDITACTION_NEWMOVABLERECT_H

#include "EditAction_NewStaticRect.h"


class EditAction_NewMovableRect : public EditAction_NewStaticRect
{
    public:
        EditAction_NewMovableRect(Level * const level);
        virtual ~EditAction_NewMovableRect();
        virtual std::string GetName() const
        {
            return "New Movable Crate";
        }
    protected:
        virtual StaticRect* NewStaticRect() const;
};

#endif // EDITACTION_NEWMOVABLERECT_H

#include "EditAction_NewMovableRect.h"
#include "MovableRect.h"

EditAction_NewMovableRect::EditAction_NewMovableRect(Level * const level) :
    EditAction_NewStaticRect(level)
{
    //ctor
}

EditAction_NewMovableRect::~EditAction_NewMovableRect()
{
    //dtor
}

StaticRect* EditAction_NewMovableRect::NewStaticRect() const
{
    return new MovableRect(mLevel);
}

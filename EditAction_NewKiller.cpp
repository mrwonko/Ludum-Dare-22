#include "EditAction_NewKiller.h"
#include "TriggerKill.h"

EditAction_NewKiller::EditAction_NewKiller(Level * const level) :
    EditAction_NewStaticRect(level)
{
    //ctor
}

EditAction_NewKiller::~EditAction_NewKiller()
{
    //dtor
}

StaticRect* EditAction_NewKiller::NewStaticRect() const
{
    return new TriggerKill(mLevel);
}

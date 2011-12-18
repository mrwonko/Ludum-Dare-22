#include "EditAction_NewLevelchange.h"
#include "TriggerLevelchange.h"

EditAction_NewLevelchange::EditAction_NewLevelchange(Level * const level) :
    EditAction_NewStaticRect(level)
{
    //ctor
}

EditAction_NewLevelchange::~EditAction_NewLevelchange()
{
    //dtor
}

StaticRect* EditAction_NewLevelchange::NewStaticRect() const
{
    return new TriggerLevelchange(mLevel);
}

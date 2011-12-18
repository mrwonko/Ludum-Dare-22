#include "Object.h"
#include "StaticRect.h"
#include "TriggerLevelchange.h"

Object::Object(Level* const level) :
    mLevel(level)
{
    //ctor
}

Object::~Object()
{
    //dtor
}

Object* Object::Create(const std::string& type, Level * const level)
{
    if(type == "Player")
    {
        std::cerr << "Trying to deserialize player! (Player handled separately)" << std::endl;
        return NULL;
    }
    if(type == "StaticRect")
    {
        return new StaticRect(level);
    }
    if(type == "TriggerLevelchange")
    {
        return new TriggerLevelchange(level);
    }
    return NULL;
}

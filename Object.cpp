#include "Object.h"

Object::Object()
{
    //ctor
}

Object::~Object()
{
    //dtor
}

Object* Object::Create(const std::string& type)
{
    if(type == "player")
    {
        std::cerr << "Trying to deserialize player! (Player handled separately)" << std::endl;
        return NULL;
    }
    return NULL;
}

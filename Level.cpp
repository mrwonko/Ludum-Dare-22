#include "Level.h"
#include "Object.h"

Level::Level()
{
    //ctor
}

Level::~Level()
{
    //clear objects
    for(std::list<Object*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        delete *it;
    }
}

const bool Level::Serialize(std::ostream& out_stream) const
{
    //serialize player
    if(!mPlayer.Serialize(out_stream))
    {
        std::cerr << "Could not serialize player!" << std::endl;
        return false;
    }

    //serialize other objects
    out_stream << mObjects.size() << "\n";
    for(std::list<Object*>::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        const Object& obj = **it;
        out_stream << "\t" << obj.GetType() << "\n";
        obj.Serialize(out_stream);
        out_stream << "\n";
    }
    return true; // successfully finished
}

const bool Level::Deserialize(std::istream& stream)
{
    //deserialize player
    if(!mPlayer.Deserialize(stream))
    {
        std::cerr << "Could not deserialize player!" << std::endl;
        return false;
    }

    //deserialize other objects
    std::list<Object*>::size_type numObjects;
    stream >> numObjects;
    for(std::list<Object*>::size_type i = 0; i < numObjects; ++i)
    {
        std::string type;
        stream >> type;
        Object* newObject = Object::Create(type);
        if(newObject == NULL)
        {
            std::cerr << "Invalid object type in level file" << std::endl;
            //cleanup done in destructor
            return false;
        }
        if(!newObject->Deserialize(stream))
        {
            std::cerr << "Could not deserialize object " << i << std::endl;
            return false;
        }
        mObjects.push_back(newObject);
    }
    return true; // successfully finished
}

void Level::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    for(std::list<Object*>::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        (*it)->Render(target, renderer);
    }
}

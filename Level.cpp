#include "Level.h"
#include "Helpers.h"
#include "Object.h"
#include "EventListenerList.h"
#include <fstream>

extern EventListenerList g_EventListeners;

static const b2Vec2 gravity(0.f, 0.f);

Level::Level(const unsigned int index) :
    mPlayer(this),
    mDebugPhysics(false),
    mWorld(gravity),
    mIndex(index)
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
    std::list<Object*>::size_type numObjects = 0;
    stream >> numObjects;
    if(stream.fail()) //if extraction failed, failbit gets set.
    {
        std::cerr << "Could not read object count!" << std::endl;
        return false;
    }
    for(std::list<Object*>::size_type i = 0; i < numObjects; ++i)
    {
        std::string type;
        stream >> type;
        if(stream.fail())
        {
            std::cerr << "Could not read object " << i << "'s type!" << std::endl;
            return false;
        }
        Object* newObject = Object::Create(type, this);
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
        target.Draw(**it);
    }

    if(mDebugPhysics)
    {
        target.Draw(mDebugDraw);
    }
}

const bool Level::ProcessEvent(const sf::Event& event)
{
    if(event.Type == sf::Event::KeyPressed)
    {
        #ifdef _DEBUG
        if(event.Key.Code == sf::Keyboard::P)
        {
            mDebugPhysics = !mDebugPhysics;
            return true;
        }
        #endif
        return false;
    }
    return false;
}

const bool Level::Load()
{
    std::ifstream file(GetLevelName(mIndex).c_str());
    if(file.fail())
    {
        std::cerr << "Could not open \"" << GetLevelName(mIndex) << "\"!" << std::endl;
        return false;
    }
    if(!Deserialize(file))
    {
        std::cerr << "Error deserializing from \"" << GetLevelName(mIndex) << "\"!" << std::endl;
        return false;
    }
    return true;
}

const bool Level::IsComplete() const
{
    //TODO
    return false;
}

#include "Level.h"
#include "Helpers.h"
#include "Object.h"
#include "UIText.h"
#include "EditAction.h"
#include <fstream>
#include "EventListenerList.h"
#include "EditAction_Click.h"

static const b2Vec2 gravity(0.f, 0.f);
extern EventListenerList g_EventListeners;

Level::Level(const unsigned int index) :
    mPlayer(this),
    mDebugPhysics(false),
    mWorld(gravity),
    mIndex(index),
    mEditMode(false)
{
    mWorld.SetAllowSleeping(true);
    mDebugDraw.SetWorld(&mWorld);
    g_EventListeners.PushBack(this);
    SetupUIs();
    SetupEditActions();
}

Level::~Level()
{
    //clear objects
    for(std::list<Object*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        delete *it;
    }
    for(EditActionList::iterator it = mEditActions.begin(); it != mEditActions.end(); ++it)
    {
        delete *it;
    }
    g_EventListeners.Remove(this);
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
    // Draw Elements
    for(std::list<Object*>::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        target.Draw(**it);
    }

    // Draw Player
    target.Draw(mPlayer);

    // Draw Debug Physics Overlay
    if(mDebugPhysics)
    {
        target.Draw(mDebugDraw);
    }

    // Draw UI
    if(mEditMode)
    {
        target.Draw(mEditUI);
    }
    else
    {
        target.Draw(mGameUI);
    }
}

const bool Level::ProcessEvent(const sf::Event& event)
{
    if(mEditMode)
    {
        //special editmode events - may overwrite base events (because they're handled first - they have priority.)
        //execute the currently selected edit action - switched via mouse wheel
        if(mCurrentEditAction != mEditActions.end()) //any edit actions?
        {
            //does the current edit action handle it?
            if((*mCurrentEditAction)->ProcessEvent(event))
            {
                return true;
            }
            if(event.Type == sf::Event::MouseWheelMoved)
            {
                if(event.MouseWheel.Delta < 0)
                {
                    ++mCurrentEditAction;
                    if(mCurrentEditAction == mEditActions.end())
                    {
                        mCurrentEditAction = mEditActions.begin();
                    }
                }
                else if(event.MouseWheel.Delta > 0)
                {
                    if(mCurrentEditAction == mEditActions.begin())
                    {
                        mCurrentEditAction = mEditActions.end();
                    }
                    --mCurrentEditAction;
                }
                OnEditActionChange(); //update text and whatnot
            }
        }
    }
    else
    {
        //gameplay events - disabled in edit mode
    }
    //events that are always handled, no matter if editmode is enabled
    if(event.Type == sf::Event::KeyPressed)
    {
        //Physics Debug Toggle
        //#ifdef _DEBUG //(Who cares if it's in release? May actually help track down bugs or something, leave it in.)
        if(event.Key.Code == Constants::PHYSDEBUG_KEY)
        {
            mDebugPhysics = !mDebugPhysics;
            return true;
        }
        //#endif
        //Editmode Toggle
        if(event.Key.Code == Constants::LEVELEDIT_KEY)
        {
            mEditMode = !mEditMode;
            return true;
        }
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

void Level::Update(unsigned int deltaT_msec)
{
    mWorld.Step(deltaT_msec / 1000.0f, PHYS_VELOCITY_ITERATIONS, PHYS_POSITION_ITERATIONS);
    for(std::list<Object*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        (*it)->Update(deltaT_msec);
    }
}

void Level::SetupUIs()
{
    UIText* actionText = new UIText;
    actionText->SetCoordinates(sf::Vector2f(0.f, 0.f));
    actionText->SetCoordinates(sf::Vector2f(-160.f, 0.f));
    mEditUI.AddElement("action", actionText);
}

void Level::SetupEditActions()
{
    //add edit actions
    mEditActions.push_back(new EditAction_Click(this));

    //set current one to first one
    assert(!mEditActions.empty());
    mCurrentEditAction = mEditActions.begin();
    OnEditActionChange();
}

void Level::OnEditActionChange()
{
    EditAction* curAction = *mCurrentEditAction;
    UIText* actionText = reinterpret_cast<UIText*>(mEditUI.GetElement("action"));
    assert(actionText != NULL);
    actionText->SetText(curAction->GetName());
}

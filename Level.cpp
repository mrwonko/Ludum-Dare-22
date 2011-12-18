#include "Level.h"
#include "Helpers.h"
#include "Object.h"
#include "UIText.h"
#include "EditAction.h"
#include <fstream>
#include <sstream>
#include "EventListenerList.h"
#include "EditAction_Click.h"
#include "EditAction_NewStaticRect.h"
#include "EditAction_NewLevelchange.h"
#include "EditAction_NewKiller.h"
#include "EditAction_Remove.h"

static const b2Vec2 gravity(0.f, Constants::GRAVITY);
extern EventListenerList g_EventListeners;
extern sf::RenderWindow* g_Window;

Level::Level(const unsigned int index) :
    mDebugPhysics(false),
    mWorld(gravity),
    mIndex(index),
    mEditMode(false),
    mPlayer(this), //after world! (.h counts)
    mStatus(Level::ePlaying)
{
    mWorld.SetAllowSleeping(true);
    mDebugDraw.SetWorld(&mWorld);
    g_EventListeners.PushBack(this);
    SetupUIs();
    SetupEditActions();
}

Level::~Level()
{
    DeleteObjects();
    for(EditActionList::iterator it = mEditActions.begin(); it != mEditActions.end(); ++it)
    {
        delete *it;
    }
    g_EventListeners.Remove(this);
}

void Level::DeleteObjects()
{
    //clear objects
    for(std::list<Object*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        delete *it;
    }
    mObjects.clear();
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
        if(event.Type == sf::Event::KeyPressed && event.Key.Code == Constants::LEVELSAVE_KEY)
        {
            if(Save())
            {
                std::cout<<"Level saved"<<std::endl;
            }
            return true;
        }
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
                (*mCurrentEditAction)->OnExit();
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
        if(mStatus == ePlaying) //cannot move player when dead
        {
            //gameplay events - disabled in edit mode
            if(mPlayer.ProcessEvent(event))
            {
                return true;
            }
        }
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
            if(mEditMode)
            {
                mEditCameraPosition = mPlayer.GetPosition();
            }
            else
            {
                SetViewPos(*g_Window, mPlayer.GetPosition());
            }
            return true;
        }
        if(event.Key.Code == Constants::RELOAD_KEY)
        {
            mStatus = ePlaying;
            DeleteObjects();
            Load();
            //TODO: Clear Particles
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

const bool Level::Save()
{
    std::stringstream temp_stream;
    if(!Serialize(temp_stream))
    {
        std::cerr << "Error serializing to \"" << GetLevelName(mIndex) << "\"!" << std::endl;
        return false;
    }
    // don't overwrite file until serialization was successful :)
    std::ofstream file(GetLevelName(mIndex).c_str(), std::ios_base::out | std::ios_base::trunc);
    if(file.fail())
    {
        std::cerr << "Could not open \"" << GetLevelName(mIndex) << "\"!" << std::endl;
        return false;
    }
    file << temp_stream.rdbuf();
    if(file.fail())
    {
        std::cerr << "Could not write to \"" << GetLevelName(mIndex) << "\"! Sorry, the file is now lost. (Hope you had a backup if you were overwriting.)" << std::endl;
        return false;
    }
    return true;
}

void Level::Update(unsigned int deltaT_msec)
{
    if(mEditMode)
    {
        if(sf::Keyboard::IsKeyPressed(Constants::MOVEL_KEY))
        {
            mEditCameraPosition.x -= Constants::EDITMODE_CAMSPEED * deltaT_msec;
        }
        if(sf::Keyboard::IsKeyPressed(Constants::MOVER_KEY))
        {
            mEditCameraPosition.x += Constants::EDITMODE_CAMSPEED * deltaT_msec;
        }
        if(sf::Keyboard::IsKeyPressed(Constants::MOVEU_KEY))
        {
            mEditCameraPosition.y -= Constants::EDITMODE_CAMSPEED * deltaT_msec;
        }
        if(sf::Keyboard::IsKeyPressed(Constants::MOVED_KEY))
        {
            mEditCameraPosition.y += Constants::EDITMODE_CAMSPEED * deltaT_msec;
        }
        assert(g_Window != NULL);
        SetViewPos(*g_Window, mEditCameraPosition);
    }
    else
    {
        // Update Physics
        mWorld.Step(deltaT_msec / 1000.0f, PHYS_VELOCITY_ITERATIONS, PHYS_POSITION_ITERATIONS);
        // Update Objects - after physics so they can update accordingly
        for(std::list<Object*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
        {
            (*it)->Update(deltaT_msec);
        }
        if(mStatus == ePlaying)
        {
            // Update Player
            mPlayer.Update(deltaT_msec);
        }
    }
}

void Level::SetupUIs()
{
    //  Game UI
    //...
    //  Edit UI
    //current action
    UIText* actionText = new UIText;
    actionText->SetCoordinates(sf::Vector2f(0.f, 0.f));
    mEditUI.AddElement("action", actionText);
}

void Level::SetupEditActions()
{
    //add edit actions
    mEditActions.push_back(new EditAction_Click(this));
    mEditActions.push_back(new EditAction_Remove(this));
    mEditActions.push_back(new EditAction_NewStaticRect(this));
    mEditActions.push_back(new EditAction_NewLevelchange(this));
    mEditActions.push_back(new EditAction_NewKiller(this));

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

void Level::RemoveObject(Object* obj)
{
    for(std::list<Object*>::iterator it = mObjects.begin(); it != mObjects.end();)
    {
        if((*it) == obj)
        {
            it = mObjects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Level::Lose()
{
    //TODO: Explosion!
    mStatus = eLost;
}

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
#include "EditAction_MovePlayer.h"
#include "EditAction_NewMovableRect.h"
#include "Sounds.h"

static const b2Vec2 gravity(0.f, Constants::GRAVITY);
extern EventListenerList g_EventListeners;
extern sf::RenderWindow* g_Window;
extern Sounds g_Sounds;

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

void Level::draw (sf::RenderTarget &target, sf::RenderStates states) const
{
    // Draw Elements
    for(std::list<Object*>::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        target.draw(**it, states);
    }

    // Draw Player
    target.draw(mPlayer, states);

    // Draw Particles
    target.draw(mParticleSystem, states);

    // Draw Debug Physics Overlay
    if(mDebugPhysics)
    {
        target.draw(mDebugDraw, states);
    }

    // Draw UI
    if(mEditMode)
    {
        target.draw(mEditUI, states);
    }
    else
    {
        if(mStatus == ePlaying)
        {
            target.draw(mGameUI, states);
        }
        else
        {
            target.draw(mGameOverUI, states);
        }
    }
}

const bool Level::ProcessEvent(const sf::Event& event)
{
    if(mEditMode)
    {
        //special editmode events - may overwrite base events (because they're handled first - they have priority.)
        if(event.type == sf::Event::KeyPressed && event.key.code == Constants::LEVELSAVE_KEY)
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
            if(event.type == sf::Event::MouseWheelMoved)
            {
                (*mCurrentEditAction)->OnExit();
                if(event.mouseWheel.delta < 0)
                {
                    ++mCurrentEditAction;
                    if(mCurrentEditAction == mEditActions.end())
                    {
                        mCurrentEditAction = mEditActions.begin();
                    }
                }
                else if(event.mouseWheel.delta > 0)
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
    if(event.type == sf::Event::KeyPressed)
    {
        //Physics Debug Toggle
        //#ifdef _DEBUG //(Who cares if it's in release? May actually help track down bugs or something, leave it in.)
        if(event.key.code == Constants::PHYSDEBUG_KEY)
        {
            mDebugPhysics = !mDebugPhysics;
            return true;
        }
        //#endif
        //Editmode Toggle
        if(event.key.code == Constants::LEVELEDIT_KEY)
        {
            mEditMode = !mEditMode;
            if(mEditMode)
            {
                mEditCameraPosition = mPlayer.getPosition();
            }
            else
            {
                SetViewPos(*g_Window, mPlayer.getPosition());
            }
            return true;
        }
        if(event.key.code == Constants::RELOAD_KEY)
        {
            mStatus = ePlaying;
            DeleteObjects();
            mPlayer.SetPosition(sf::Vector2f(0.f, 0.f)); //in case loading fails due to unsafed map
            Load();
            mParticleSystem.Clear();
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
    mChannel1.setBuffer(g_Sounds.Start);
    mChannel1.play();
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
        if(sf::Keyboard::isKeyPressed(Constants::MOVEL_KEY))
        {
            mEditCameraPosition.x -= Constants::EDITMODE_CAMSPEED * deltaT_msec;
        }
        if(sf::Keyboard::isKeyPressed(Constants::MOVER_KEY))
        {
            mEditCameraPosition.x += Constants::EDITMODE_CAMSPEED * deltaT_msec;
        }
        if(sf::Keyboard::isKeyPressed(Constants::MOVEU_KEY))
        {
            mEditCameraPosition.y -= Constants::EDITMODE_CAMSPEED * deltaT_msec;
        }
        if(sf::Keyboard::isKeyPressed(Constants::MOVED_KEY))
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
        // Update particles
        mParticleSystem.Update(deltaT_msec);
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

    UIText* helpText = new UIText;
    helpText->SetCoordinates(sf::Vector2f(0.f, -200.f));
    helpText->SetText("F2 and F3 change the level\nF5 saves\nR reloads (save first!)\nF6 adds a new level (at the end)\nHold control to snap\nScroll mousewheel to change tool");
    mEditUI.AddElement("help", helpText);

    UIText* editmodeText = new UIText;
    editmodeText->SetCoordinates(sf::Vector2f(-120.f, -35.f));
    editmodeText->SetText("Edit Mode");
    mEditUI.AddElement("editmode", editmodeText);

    UIText* levelText = new UIText;
    std::stringstream ss;
    ss << "Level " << mIndex;
    levelText->SetText(ss.str());
    levelText->SetCoordinates(sf::Vector2f(-100.f, 0.f));
    mEditUI.AddElement("level", levelText);

    //  Game Over UI
    UIText* retryText = new UIText;
    retryText->SetText("Press R to retry");
    retryText->SetCoordinates(sf::Vector2f(0.f, 0.f));
    mGameOverUI.AddElement("retry", retryText);
}

void Level::SetupEditActions()
{
    //add edit actions
    mEditActions.push_back(new EditAction_Click(this));
    mEditActions.push_back(new EditAction_Remove(this));
    mEditActions.push_back(new EditAction_NewStaticRect(this));
    mEditActions.push_back(new EditAction_NewMovableRect(this));
    mEditActions.push_back(new EditAction_NewKiller(this));
    mEditActions.push_back(new EditAction_NewLevelchange(this));
    mEditActions.push_back(new EditAction_MovePlayer(this));

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
    mParticleSystem.CreateExplosion(mPlayer.getPosition());
    mStatus = eLost;
    mChannel1.setBuffer(g_Sounds.Death);
    mChannel1.play();
    mPlayer.Stop(); // or the physical body would move further, potentially dying again or even winning.
}

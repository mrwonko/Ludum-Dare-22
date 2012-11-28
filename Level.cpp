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
#include "EditAction_Remove.h"
#include "EditAction_MovePlayer.h"
#include "EditAction_NewPlayer.h"
#include "Sounds.h"
#include <SFML/Network/Packet.hpp>

static const b2Vec2 gravity(0.f, Constants::GRAVITY);
extern EventListenerList g_EventListeners;
extern sf::RenderWindow* g_Window;
extern Sounds g_Sounds;

Level::Level(const bool editMode) :
    Ready(false),
    mDebugPhysics(false),
    mWorld(gravity),
    mEditMode(editMode),
    mStatus(Level::ePlanning),
    mIndex(-1)
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
    DeletePlayers();
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

void Level::DeletePlayers()
{
    //clear objects
    for(std::vector<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
    {
        delete *it;
    }
    mPlayers.clear();
}

const bool Level::Serialize(std::ostream& out_stream) const
{
    //serialize players
    out_stream << mPlayers.size() << "\n";
    for(std::vector<Player*>::const_iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
    {
        Player& player = **it;
        if(!player.Serialize(out_stream))
        {
            std::cerr << "Could not serialize a player!" << std::endl;
            return false;
        }
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

const bool Level::Serialize(sf::Packet& out_packet) const
{
    //serialize players
    out_packet << mPlayers.size();
    for(std::vector<Player*>::const_iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
    {
        Player& player = **it;
        if(!player.Serialize(out_packet))
        {
            std::cerr << "Could not serialize a player!" << std::endl;
            return false;
        }
    }

    //serialize other objects
    out_packet << mObjects.size();
    for(std::list<Object*>::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        const Object& obj = **it;
        out_packet << obj.GetType();
        obj.Serialize(out_packet);
    }
    return true; // successfully finished
}

const bool Level::Deserialize(std::istream& stream)
{
    //deserialize players
    unsigned int numPlayers;
    stream >> numPlayers;
    if(stream.fail())
    {
        std::cerr<<"Could not read player count!" << std::endl;
        return false;
    }
    for(unsigned int i = 0 ;  i < numPlayers  ; ++i)
    {
        Player* player = new Player(this);
        player->SetIndex(i);
        if(!player->Deserialize(stream))
        {
            std::cerr << "Could not deserialize player " << i+1 << "!" << std::endl;
            delete player;
            return false;
        }
        mPlayers.push_back(player);
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

const bool Level::Deserialize(sf::Packet& packet)
{
    //deserialize players
    unsigned int numPlayers;
    packet >> numPlayers;
    if(!packet)
    {
        std::cerr<<"Could not read player count!" << std::endl;
        return false;
    }
    for(unsigned int i = 0 ;  i < numPlayers  ; ++i)
    {
        Player* player = new Player(this);
        player->SetIndex(i);
        if(!player->Deserialize(packet))
        {
            std::cerr << "Could not deserialize player " << i+1 << "!" << std::endl;
            delete player;
            return false;
        }
        mPlayers.push_back(player);
    }

    //deserialize other objects
    std::list<Object*>::size_type numObjects = 0;
    packet >> numObjects;
    if(!packet)
    {
        std::cerr << "Could not read object count!" << std::endl;
        return false;
    }
    for(std::list<Object*>::size_type i = 0; i < numObjects; ++i)
    {
        std::string type;
        packet >> type;
        if(!packet)
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
        if(!newObject->Deserialize(packet))
        {
            std::cerr << "Could not deserialize object " << i << std::endl;
            delete newObject;
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

    // Draw Players
    for(std::vector<Player*>::const_iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
    {
        target.Draw(**it);
    }

    // Draw Particles
    target.Draw(mParticleSystem);

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
        if(!mPlayers[mIndex]->IsDead())
        {
            if(mStatus == ePlanning)
            {
                target.Draw(mGameUI);
            }
            else
            {
                target.Draw(mWaitingUI);
            }
        }
        else
        {
            if(mStatus != eOutcome) //don't show game over before you're dead!
            {
                target.Draw(mGameOverUI);
            }
        }
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
                return true;
            }
        }
        else
        {
            std::cout<<"Warning: no edit actions!"<<std::endl;
        }
    }
    else
    {
        //gameplay events - disabled in edit mode
        if(mIndex != -1)
        {
            if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Return)
            {
                Ready = true;
                mStatus = eWaiting;
            }
            else if(mPlayers[mIndex]->ProcessEvent(event))
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
    }
    return false;
}

const bool Level::Load()
{
    std::ifstream file(mFilename.c_str());
    if(file.fail())
    {
        std::cerr << "Could not open \"" << mFilename << "\"!" << std::endl;
        return false;
    }
    if(!Deserialize(file))
    {
        std::cerr << "Error deserializing from \"" << mFilename << "\"!" << std::endl;
        return false;
    }
    mChannel1.SetBuffer(g_Sounds.Start);
    mChannel1.Play();
    return true;
}

const bool Level::Save()
{
    std::stringstream temp_stream;
    if(!Serialize(temp_stream))
    {
        std::cerr << "Error serializing to \"" << mFilename << "\"!" << std::endl;
        return false;
    }
    // don't overwrite file until serialization was successful :)
    std::ofstream file(mFilename.c_str(), std::ios_base::out | std::ios_base::trunc);
    if(file.fail())
    {
        std::cerr << "Could not open \"" << mFilename << "\"!" << std::endl;
        return false;
    }
    file << temp_stream.rdbuf();
    if(file.fail())
    {
        std::cerr << "Could not write to \"" << mFilename << "\"! Sorry, the file is now lost. (Hope you had a backup if you were overwriting.)" << std::endl;
        return false;
    }
    return true;
}

void Level::Update(unsigned int deltaT_msec)
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

    if(mStatus == eOutcome)
    {
        mTimeToNextOutcomeFrame -= deltaT_msec;
        while(mTimeToNextOutcomeFrame < 0)
        {
            CalculateOutcomeFrame();
        }
    }

}

void Level::SetupUIs()
{
    //  Game UI
    //...

    UIText* waitingText = new UIText;
    waitingText->SetCoordinates(sf::Vector2f(0.f, 0.f));
    waitingText->SetText("Done. Waiting for opponents...");
    mWaitingUI.AddElement("waiting", waitingText);

    //  Edit UI
    //current action
    UIText* actionText = new UIText;
    actionText->SetCoordinates(sf::Vector2f(0.f, 0.f));
    mEditUI.AddElement("action", actionText);

    UIText* helpText = new UIText;
    helpText->SetCoordinates(sf::Vector2f(0.f, -200.f));
    helpText->SetText("F5 saves\n\nHold control to snap\nScroll mousewheel to change tool");
    mEditUI.AddElement("help", helpText);

    UIText* editmodeText = new UIText;
    editmodeText->SetCoordinates(sf::Vector2f(-120.f, -35.f));
    editmodeText->SetText("Edit Mode");
    mEditUI.AddElement("editmode", editmodeText);

    UIText* levelText = new UIText;
    std::stringstream ss;
    ss << "Filename: " << mFilename;
    levelText->SetText(ss.str());
    levelText->SetCoordinates(sf::Vector2f(0.f, 50.f));
    mEditUI.AddElement("level", levelText);

    //  Game Over UI
    UIText* retryText = new UIText;
    retryText->SetText("You're le dead.");
    retryText->SetCoordinates(sf::Vector2f(0.f, 0.f));
    mGameOverUI.AddElement("retry", retryText);
}

void Level::SetupEditActions()
{
    //add edit actions
    mEditActions.push_back(new EditAction_Click(this));
    mEditActions.push_back(new EditAction_Remove(this));
    mEditActions.push_back(new EditAction_NewStaticRect(this));
    mEditActions.push_back(new EditAction_MovePlayer(this));
    mEditActions.push_back(new EditAction_NewPlayer(this));

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

const unsigned int Level::GetNumPlayers() const
{
    return mPlayers.size(); //TODO: numplayers
}

const bool Level::SetNumPlayers(const unsigned int count)
{
    if(count > mPlayers.size())
    {
        std::cerr<<"Cannot create additional players in level!"<<std::endl;
        return false;
    }
    for(std::vector<Player*>::iterator it = mPlayers.begin() + count; it != mPlayers.end(); ++it)
    {
        delete *it;
    }
    mPlayers.erase(mPlayers.begin() + count, mPlayers.end());
    assert(mPlayers.size() == count);
    return true;
}

void Level::AddPlayer(Player* player)
{
    player->SetIndex(mPlayers.size());
    mPlayers.push_back(player);
}

void Level::RemovePlayer(const unsigned int index)
{
    if( index >= mPlayers.size()) return;
    delete mPlayers[index];
    for(unsigned int i = index + 1; i < mPlayers.size(); ++i)
    {
        mPlayers[i]->SetIndex(i-1);
        mPlayers[i-1] = mPlayers[i];
    }
    mPlayers.erase(mPlayers.end()-1);
}

void Level::ShowOutcome()
{
    mStatus = eOutcome;
    mOutcomeFramesLeft = Constants::FRAMES_PER_ROUND;
    mTimeToNextOutcomeFrame = 0;
}

void Level::CalculateOutcomeFrame()
{
    mTimeToNextOutcomeFrame += Constants::MSEC_PER_FRAME;
    mOutcomeFramesLeft -= 1;
    if(mOutcomeFramesLeft == 0)
    {
        mStatus = ePlanning;
    }

    // Update Physics
    mWorld.Step(Constants::MSEC_PER_FRAME / 1000.0f, PHYS_VELOCITY_ITERATIONS, PHYS_POSITION_ITERATIONS);
    // Update Objects - after physics so they can update accordingly
    for(std::list<Object*>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
        (*it)->Update(Constants::MSEC_PER_FRAME);
    }
    for(std::vector<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
    {
        (*it)->Update(Constants::MSEC_PER_FRAME);
    }
    // Update particles
    mParticleSystem.Update(Constants::MSEC_PER_FRAME);
}

void Level::CalculateOutcome()
{
    for(unsigned int i = 0; i < Constants::FRAMES_PER_ROUND; ++i)
    {
        CalculateOutcomeFrame();
    }
}

const bool Level::IsOver() const
{
    unsigned int numAlive = 0;
    for(std::vector<Player*>::const_iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
    {
        if(!(**it).IsDead())
        {
            ++numAlive;
        }
    }
    return numAlive < 2;
}

#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "Player.h"
#include "Constants.h"
#include "EventListener.h"
#include "sfmlBox2DDebugDraw.h"
#include <SFML/Graphics.hpp>
#include "UI.h"
#include "ParticleSystem.h"
#include <SFML/Audio/Sound.hpp>

class EditAction;
class Object;
namespace sf
{
    class Packet;
}

/** This turned into way more than just the level, it's a kind of general game class **/
class Level : public sf::Drawable, public EventListener
{
    static const int PHYS_VELOCITY_ITERATIONS = 6;
    static const int PHYS_POSITION_ITERATIONS = 2;

    enum Status
    {
        ePlaying,
        eWon,
        eLost
    };

    public:
        Level(const bool editMode = false);
        virtual ~Level();

        /** \brief Loads the level (index set in constructor)
            \return success
        **/
        const bool Load();

        /** \brief Saves the level (index set in constructor)
            \return success
        **/
        const bool Save();

        void Update(unsigned int deltaT_msec);

        void AddObject(Object* obj) ///< Fire & Forget - will delete objects on destruction. (Or reload)
        {
            mObjects.push_back(obj);
        }

        void RemoveObject(Object* obj); ///< Only erases from list, does not delete object. i.e. you become responsible for deleting it. (Well, since you are me and only I work on this, you area already responsible...)

        b2World& GetWorld() { return mWorld; }

        virtual const bool ProcessEvent(const sf::Event& event);

        const bool InEditMode() const { return mEditMode; }

        ParticleSystem& GetParticleSystem() { return mParticleSystem; }

        Player* GetPlayer(const unsigned int index) { if(index < mPlayers.size()) return mPlayers[index]; else return NULL; }

        void SetFilename(const std::string& filename) { mFilename = filename; }

        const unsigned int GetNumPlayers() const;

        const bool SetNumPlayers(const unsigned int count); //deletes unnecessary players (but cannot create new ones!)

        const bool Serialize(sf::Packet& out_packet) const;
        const bool Deserialize(sf::Packet& packet);

        void SetIndex(const int index) { mIndex = index; }

    private:
        void DeleteObjects();
        void DeletePlayers();

        /** \return success
            \param out_stream Stream to write into
        **/
        const bool Serialize(std::ostream& out_stream) const;

        /** \return success
            \param stream Stream to read from
        **/
        const bool Deserialize(std::istream& stream);

        /// SFML Rendering function
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;

        void SetupUIs();
        void SetupEditActions();
        void OnEditActionChange();

        std::list<Object*> mObjects; ///< Everything but the player
        bool mDebugPhysics; ///< Show physics debug drawing? (toggle with P)
        sfmlBox2DDebugDraw mDebugDraw; ///< Physics debug drawing
        b2World mWorld; ///< Physical World
        std::string mFilename; ///< Which level is this?
        bool mEditMode; ///< Whether we're currently in the level editing mode
        std::vector<Player*> mPlayers; ///< The Players
        sf::Text mEditText;
        UI mEditUI;
        UI mGameUI; ///< \note I shouldn't create a new one for each level... But this is Ludum Dare, screw good design XD (I don't actually have any ingame UI)
        UI mGameOverUI;
        typedef std::list<EditAction*> EditActionList;
        EditActionList mEditActions;
        EditActionList::iterator mCurrentEditAction;
        sf::Vector2f mEditCameraPosition;
        Status mStatus;
        ParticleSystem mParticleSystem;
        sf::Sound mChannel1;

        int mIndex;
};

#endif // LEVEL_H

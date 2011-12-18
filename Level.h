#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <list>
#include "Player.h"
#include "Constants.h"
#include "EventListener.h"
#include "sfmlBox2DDebugDraw.h"
#include <SFML/Graphics.hpp>
#include "UI.h"

class EditAction;
class Object;

class Level : public sf::Drawable, public EventListener
{
    static const int PHYS_VELOCITY_ITERATIONS = 6;
    static const int PHYS_POSITION_ITERATIONS = 2;

    public:
        Level(const unsigned int index);
        virtual ~Level();

        /** \brief Loads the level (index set in constructor)
            \return success
        **/
        const bool Load();

        /** \brief Saves the level (index set in constructor)
            \return success
        **/
        const bool Save();

        const bool IsComplete() const;

        void Update(unsigned int deltaT_msec);

        void AddObject(Object* obj)
        {
            mObjects.push_back(obj);
        }

        b2World& GetWorld() { return mWorld; }

        virtual const bool ProcessEvent(const sf::Event& event);

        const bool InEditMode() const { return mEditMode; }

    private:
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
        Player mPlayer; ///< The Player
        bool mDebugPhysics; ///< Show physics debug drawing? (toggle with P)
        sfmlBox2DDebugDraw mDebugDraw; ///< Physics debug drawing
        b2World mWorld; ///< Physical World
        const unsigned int mIndex; ///< Which level is this?
        bool mEditMode; ///< Whether we're currently in the level editing mode
        sf::Text mEditText;
        UI mEditUI;
        UI mGameUI; ///< \note I shouldn't create a new one for each level... But this is Ludum Dare, screw good design XD
        typedef std::list<EditAction*> EditActionList;
        EditActionList mEditActions;
        EditActionList::iterator mCurrentEditAction;
};

#endif // LEVEL_H

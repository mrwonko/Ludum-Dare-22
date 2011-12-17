#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <list>
#include "Player.h"
#include "sfmlBox2DDebugDraw.h"
#include "EventListener.h"
#include <SFML/Graphics.hpp>

class Object;

class Level : public sf::Drawable, public EventListener
{
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

        std::list<Object*> mObjects; ///< Everything but the player
        Player mPlayer; ///< The Player
        bool mDebugPhysics; ///< Show physics debug drawing? (toggle with P)
        sfmlBox2DDebugDraw mDebugDraw; ///< Physics debug drawing
        b2World mWorld; ///< Physical World
        const unsigned int mIndex; ///< Which level is this?
};

#endif // LEVEL_H

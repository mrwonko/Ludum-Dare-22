#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <list>
#include "Player.h"
#include "sfmlBox2DDebugDraw.h"
#include <SFML/Graphics.hpp>

class Object;

class Level : public sf::Drawable
{
    public:
        Level();
        virtual ~Level();

        void Update(unsigned int deltaT_msec);

        /** \return success
            \param out_stream Stream to write into
        **/
        const bool Serialize(std::ostream& out_stream) const;

        /** \return success
            \param stream Stream to read from
        **/
        const bool Deserialize(std::istream& stream);

        void AddObject(Object* obj)
        {
            mObjects.push_back(obj);
        }

        b2World& GetWorld() { return mWorld; }

    private:
        // everything BUT THE PLAYER (although he's an object, too.)
        std::list<Object*> mObjects;
        Player mPlayer;
        sfmlBox2DDebugDraw mDebugDraw;
        b2World mWorld;

        // SFML Rendering function
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
};

#endif // LEVEL_H

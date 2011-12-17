#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <list>
#include "Player.h"
#include <SFML/Graphics.hpp>

class Object;

class Level : public sf::Drawable
{
    public:
        Level();
        virtual ~Level();

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

    private:
        // everything BUT THE PLAYER (although he's an object, too.)
        std::list<Object*> mObjects;
        Player mPlayer;

        // SFML Rendering function
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
};

#endif // LEVEL_H

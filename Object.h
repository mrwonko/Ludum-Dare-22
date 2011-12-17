#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class Level;

/** Pretty much everything is an object. That way I can store pointers to it in box2D userdata. Can be casted accordingly using GetType().

    Since in Editor-Mode every object needs to be drawn, I simply make them all drawable. Also keeps things simpler and prevents diamonds of death (PhysicalObject and DrawableObject)
**/
class Object : public sf::Drawable
{
    public:
        virtual ~Object();

        /** Returns a new object of the given type or NULL for invalid types **/
        static Object* Create(const std::string& type, Level * const level);

        /** \return success
            \param out_stream Stream to write into
        **/
        virtual const bool Serialize(std::ostream& out_stream) const = 0;

        /** \return success
            \param stream Stream to read from
        **/
        virtual const bool Deserialize(std::istream& stream) = 0;

        /** Returns the object's type (for serialization)
        **/
        virtual const std::string GetType() const = 0;

        /** Called when this object is clicked in Edit mode
        **/
        virtual void Edit_OnClicked(const sf::Mouse::Button button) {}

    protected:
        Object(Level* const level);

        Level* const mLevel;
};

#endif // OBJECT_H

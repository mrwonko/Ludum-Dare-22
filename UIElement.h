#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <SFML/Graphics/Drawable.hpp>

class UI;

class UIElement : public sf::Drawable
{
    public:
        UIElement();
        virtual ~UIElement();

        void SetCoordinates(const sf::Vector2f& coordinates) { mCoordinates = coordinates; }
        const sf::Vector2f& GetCoordinates() const { return mCoordinates; }

    protected:
        /// The screen coordinates, where negative means measured from the other side in opposite direction. Not to be confused with position, the actual position
        sf::Vector2f mCoordinates;
    private:
};

#endif // UIELEMENT_H

#ifndef STATICRECT_H
#define STATICRECT_H

#include "Object.h"
#include "EventListener.h"

class b2Body;

class StaticRect : public Object //, public EventListener //event listener: may need this if I want to react to different events in edit mode?
{
    public:
        StaticRect(Level* const level);
        virtual ~StaticRect();

        virtual const std::string GetType() const { return "StaticRect"; }

        virtual const bool Serialize(std::ostream& out_stream) const;
        virtual const bool Deserialize(std::istream& stream);

    protected:
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;

        sf::Vector2f mCorner1;
        sf::Vector2f mCorner2;

        sf::Shape mShape;
        b2Body* mBody;
};

#endif // STATICRECT_H

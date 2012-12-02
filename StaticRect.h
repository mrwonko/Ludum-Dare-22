#ifndef STATICRECT_H
#define STATICRECT_H

#include "Object.h"
#include "EventListener.h"

class b2Body;
class b2Vec2;
class b2Fixture;

class StaticRect : public Object //, public EventListener //event listener: may need this if I want to react to different events in edit mode?
{
    public:
        StaticRect(Level* const level);
        virtual ~StaticRect();

        virtual const std::string GetType() const { return "StaticRect"; }

        virtual const bool Serialize(std::ostream& out_stream) const;
        virtual const bool Deserialize(std::istream& stream);

        void SetCorner1(const sf::Vector2f& pos);
        void SetCorner2(const sf::Vector2f& pos);
        void SetColor(const sf::Color& color); ///< Default: White

        virtual void Edit_OnClicked(const sf::Mouse::Button button);

    protected:
		void setPosition(const sf::Vector2f&); //renamed in SFML to this - not implementing this so I get an error if I accidentally call it
        virtual void UpdateShape();
		
		virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;

        sf::Vector2f mCorner1;
        sf::Vector2f mCorner2;

        sf::RectangleShape mShape;
        unsigned int mColorIndex;
        sf::Color mColor;
        b2Body* mBody;
        b2Fixture* mFixture;

        static std::vector<sf::Color> sColors;

        virtual void CreateBody(const b2Vec2& pos);

        virtual void CreateFixture(const float hsizeX, const float hsizeY);
};

#endif // STATICRECT_H

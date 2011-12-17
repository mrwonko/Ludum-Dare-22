#ifndef SFMLBOX2DDEBUGDRAW_H
#define SFMLBOX2DDEBUGDRAW_H

#include <SFML/Graphics/Drawable.hpp>
#include <Box2D/Box2D.h>


// this whole class is probably overkill and doesn't belong in a ludum dare but physics debug sounds damn useful to me
class sfmlBox2DDebugDraw : public sf::Drawable, public b2Draw
{
    public:
        sfmlBox2DDebugDraw();

        void SetWorld(b2World* newWorld);

        virtual ~sfmlBox2DDebugDraw();

        ///Draw a closed polygon provided in CCW order.
        virtual void 	DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
        ///Draw a solid closed polygon provided in CCW order.
        virtual void 	DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
        ///Draw a circle.
        virtual void 	DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color);
        ///Draw a solid circle.
        virtual void 	DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color);
        ///Draw a line segment.
        virtual void 	DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);
        virtual void 	DrawTransform (const b2Transform &xf);
    private:
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
        b2World* mWorld;
        sf::RenderTarget* mTarget;
        sf::Renderer* mRenderer;

        void ProcessVertex(const sf::Vector2f& pos);
};

#endif // SFMLBOX2DDEBUGDRAW_H

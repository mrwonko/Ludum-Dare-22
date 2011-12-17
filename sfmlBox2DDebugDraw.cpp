#include "sfmlBox2DDebugDraw.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <Box2d/Common/b2Math.h>
#include <gl/gl.h>
#include <cmath>

sfmlBox2DDebugDraw::sfmlBox2DDebugDraw():
    mWorld(NULL),
    mTarget(NULL),
    mRenderer(NULL)
{
    //ctor
    SetFlags(e_shapeBit | e_jointBit | e_aabbBit);
}

sfmlBox2DDebugDraw::~sfmlBox2DDebugDraw()
{
    //dtor
}

void sfmlBox2DDebugDraw::SetWorld(b2World* newWorld)
{
    if(mWorld)
    {
        mWorld->SetDebugDraw(NULL);
    }
    mWorld = newWorld;
    mWorld->SetDebugDraw(this);
}

void sfmlBox2DDebugDraw::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    //I need to hack around the const to be able to use these in the Box2D Callbacks.
    sfmlBox2DDebugDraw* unconstThis = const_cast<sfmlBox2DDebugDraw*>(this);
    unconstThis->mTarget = &target;
    unconstThis->mRenderer = &renderer;

    mWorld->DrawDebugData();
}

//some box2d -> sfml helpers
namespace
{
    sf::Color ConvertColor(const b2Color& color)
    {
        return sf::Color(255 * color.r, 255 * color.g, 255 * color.b);
    }

    sf::Vector2f ConvertVector(const b2Vec2& vec)
    {
        return sf::Vector2f(vec.x, vec.y);
    }
}

///Draw a closed polygon provided in CCW order.
void sfmlBox2DDebugDraw::DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    mRenderer->SetColor(ConvertColor(color));
    glBegin(GL_LINE_LOOP);
    for(int32 i = 0; i < vertexCount; ++i)
    {
        ProcessVertex(sf::Vector2f(vertices[i].x, vertices[i].y));
    }
    glEnd();
}

///Draw a solid closed polygon provided in CCW order.
void sfmlBox2DDebugDraw::DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    //I make them transparent.
    sf::Color col = ConvertColor(color);
    col.a = 127;
    mRenderer->SetColor(col);
    mRenderer->Begin(sf::Renderer::TriangleFan);
    for(int32 i = 0; i < vertexCount; ++i)
    {
        mRenderer->AddVertex(vertices[i].x, vertices[i].y);
    }
    mRenderer->End();

}

static const int CIRCLE_ITERATIONS = 16;

///Draw a circle.
void sfmlBox2DDebugDraw::DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color)
{
    mRenderer->SetColor(ConvertColor(color));
    mRenderer->SaveGLStates();
    glBegin(GL_LINE_LOOP);
    for(int32 i = 0; i < CIRCLE_ITERATIONS; ++i)
    {
        float rad = 2.f * M_PI * float(i) / float(CIRCLE_ITERATIONS);
        sf::Vector2f pos = ConvertVector(center) + radius * sf::Vector2f(cos(rad), sin(rad));
        ProcessVertex(pos);
    }
    glEnd();
}

///Draw a solid circle.
void sfmlBox2DDebugDraw::DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
    //I make them transparent.
    sf::Color col = ConvertColor(color);
    col.a = 127;
    mRenderer->SetColor(col);
    mRenderer->SetColor(col);
    mRenderer->Begin(sf::Renderer::TriangleFan);
    for(int32 i = 0; i < CIRCLE_ITERATIONS; ++i)
    {
        float rad = 2.f * M_PI * float(i) / float(CIRCLE_ITERATIONS);
        b2Vec2 pos = center + radius * b2Vec2(cos(rad), sin(rad));
        mRenderer->AddVertex(pos.x, pos.y);
    }
    mRenderer->End();
}

///Draw a line segment.
void sfmlBox2DDebugDraw::DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    sf::Color col = ConvertColor(color);
    mRenderer->SetColor(col);
    glBegin(GL_LINES);
    ProcessVertex(ConvertVector(p1));
    ProcessVertex(ConvertVector(p2));
    glEnd();
}

void sfmlBox2DDebugDraw::DrawTransform (const b2Transform &xf)
{
    //whatever
}

void sfmlBox2DDebugDraw::ProcessVertex(const sf::Vector2f& pos)
{
    sf::Vector2f pos2 = GetMatrix().Transform(pos);
    glVertex2f(pos2.x, pos2.y);
}

#include "sfmlBox2DDebugDraw.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <Box2d/Common/b2Math.h>
#include <cmath>
#include <vector>
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

sfmlBox2DDebugDraw::sfmlBox2DDebugDraw():
    mWorld(NULL),
    mTarget(NULL)
{
    //ctor
    SetFlags(e_shapeBit | e_jointBit | e_aabbBit);
    AppendFlags(e_centerOfMassBit);
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

void sfmlBox2DDebugDraw::draw (sf::RenderTarget &target, sf::RenderStates states) const
{
    //I need to hack around the const to be able to use these in the Box2D Callbacks.
    sfmlBox2DDebugDraw* unconstThis = const_cast<sfmlBox2DDebugDraw*>(this);
    unconstThis->mTarget = &target;
	unconstThis->mStates = states;

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
	std::vector< sf::Vertex > sfVertices;
	const sf::Color sfColor = ConvertColor( color );
	sfVertices.reserve( vertexCount > 0 ? vertexCount + 1 : 0 );
    for(int32 i = 0; i < vertexCount; ++i)
    {
		sfVertices.push_back( sf::Vertex( ConvertVector( vertices[i] ), sfColor ) );
    }
	if( vertexCount > 0 )
	{
		sfVertices.push_back( sfVertices[ 0 ] );
	}
	mTarget->draw( sfVertices.data(), sfVertices.size(), sf::LinesStrip, mStates );
}

///Draw a solid closed polygon provided in CCW order.
void sfmlBox2DDebugDraw::DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	std::vector< sf::Vertex > sfVertices;
	sf::Color sfColor = ConvertColor( color );
	sfColor.a = 127;
	sfVertices.reserve( vertexCount );
    for(int32 i = 0; i < vertexCount; ++i)
    {
		sfVertices.push_back( sf::Vertex( ConvertVector( vertices[i] ), sfColor ) );
    }
	mTarget->draw( sfVertices.data(), sfVertices.size(), sf::TrianglesFan, mStates );
}

static const int CIRCLE_ITERATIONS = 16;

///Draw a circle.
void sfmlBox2DDebugDraw::DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color)
{
	std::vector< sf::Vertex > sfVertices;
	sf::Color sfColor = ConvertColor( color );
	assert( CIRCLE_ITERATIONS > 0 );
	sfVertices.reserve( CIRCLE_ITERATIONS + 1 );
    for(int32 i = 0; i < CIRCLE_ITERATIONS; ++i)
    {
        float rad = 2.f * M_PI * float(i) / float(CIRCLE_ITERATIONS);
        sf::Vector2f pos = ConvertVector(center) + radius * sf::Vector2f(cos(rad), sin(rad));
		sfVertices.push_back( sf::Vertex( pos, sfColor ) );
    }
	sfVertices.push_back( sfVertices[0] );
	mTarget->draw( sfVertices.data(), sfVertices.size(), sf::LinesStrip, mStates );
}

///Draw a solid circle.
void sfmlBox2DDebugDraw::DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
	std::vector< sf::Vertex > sfVertices;
	sf::Color sfColor = ConvertColor( color );
	sfColor.a = 127;
	sfVertices.reserve( CIRCLE_ITERATIONS );
    for(int32 i = 0; i < CIRCLE_ITERATIONS; ++i)
    {
        float rad = 2.f * M_PI * float(i) / float(CIRCLE_ITERATIONS);
        sf::Vector2f pos = ConvertVector(center) + radius * sf::Vector2f(cos(rad), sin(rad));
		sfVertices.push_back( sf::Vertex( pos, sfColor ) );
    }
	mTarget->draw( sfVertices.data(), sfVertices.size(), sf::TrianglesFan, mStates );
}

///Draw a line segment.
void sfmlBox2DDebugDraw::DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
	std::vector< sf::Vertex > sfVertices;
	sf::Color sfColor = ConvertColor( color );
	sfVertices.reserve( 2 );
	sfVertices.push_back( sf::Vertex( ConvertVector( p1 ), sfColor ) );
	sfVertices.push_back( sf::Vertex( ConvertVector( p2 ), sfColor ) );
	mTarget->draw( sfVertices.data(), sfVertices.size(), sf::Lines, mStates );
}

void sfmlBox2DDebugDraw::DrawTransform (const b2Transform &xf)
{
    static const b2Color col(1.f, 0.f, 0.f);
    DrawSegment(xf.p, xf.p + 2 * b2Vec2(xf.q.c, xf.q.s), col);
}
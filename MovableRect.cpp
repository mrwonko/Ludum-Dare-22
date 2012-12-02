#include "MovableRect.h"
#include "Level.h"
#include <Box2D/Box2D.h>
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

MovableRect::MovableRect(Level * const level):
    StaticRect(level)
{
    //ctor
}

MovableRect::~MovableRect()
{
    //dtor
}

void MovableRect::CreateBody(const b2Vec2& pos)
{
    if(!mBody)
    {
        b2BodyDef def;
        def.position = pos;
        def.userData = this;
        def.type = b2_dynamicBody; //dynamic!
        def.allowSleep = true;
        mBody = mLevel->GetWorld().CreateBody(&def);
    }
    else
    {
        mBody->SetTransform(pos, 0);
    }
}

void MovableRect::CreateFixture(const float hsizeX, const float hsizeY)
{
    if(hsizeX < 0.01f) return; //Box2D crashes when determining the weight of a size 0 object
    if(hsizeY < 0.01f) return;
    if(mFixture)
    {
        mBody->DestroyFixture(mFixture);
    }
    b2PolygonShape shape;
    shape.SetAsBox(hsizeX, hsizeY);
    b2FixtureDef def;
    def.shape = &shape;
    def.density = 2.f; // less dense than the player
    def.friction = 0.3f;
    mFixture = mBody->CreateFixture(&def); // second param is mass, not used here
}

void MovableRect::Update(unsigned int deltaT_msec)
{
    if(mBody->IsAwake())
    {
        mShape.setPosition(mBody->GetPosition().x, mBody->GetPosition().y);
        mShape.setRotation(180.f / M_PI * mBody->GetAngle());
    }
}

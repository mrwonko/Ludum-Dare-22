#include "Trigger.h"
#include "Level.h"

Trigger::Trigger(Level* const level) :
    StaticRect(level)
{
    //ctor
}

Trigger::~Trigger()
{
    //dtor
}

void Trigger::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    if(mLevel->InEditMode())
    {
        target.Draw(mShape);
    }
}

void Trigger::UpdateShape()
{
    sf::Vector2f size = mCorner2 - mCorner1;
    sf::Vector2f center = mCorner1 + 0.5f * size ;
    mShape = sf::Shape::Rectangle(mCorner1.x, mCorner1.y, size.x, size.y, mColor);
    mShape.EnableFill(true);
    mShape.EnableOutline(false);
    if(!mBody)
    {
        b2BodyDef def;
        def.position.Set(center.x, center.y);
        def.userData = this;
        mBody = mLevel->GetWorld().CreateBody(&def);
    }
    else
    {
        mBody->SetTransform(b2Vec2(center.x, center.y), mBody->GetAngle());
    }
    if(mFixture)
    {
        mBody->DestroyFixture(mFixture);
    }
    b2PolygonShape shape;
    shape.SetAsBox(std::abs(size.x/2), std::abs(size.y/2));
    b2FixtureDef fixDef;
    fixDef.shape = &shape;
    fixDef.isSensor = true; //No collision!
    mFixture = mBody->CreateFixture(&fixDef); // second param is mass, not used here
}

void Trigger::Update(unsigned int deltaT_msec)
{
    //todo
}

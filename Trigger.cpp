#include "Trigger.h"
#include "Level.h"

Trigger::Trigger(Level* const level) :
    StaticRect(level),
    mPlayerWasInside(false),
    mIsSensor(true)
{
    //ctor
}

Trigger::~Trigger()
{
    //dtor
}

void Trigger::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(mLevel->InEditMode())
    {
		states.transform *= getTransform();
        target.draw(mShape, states);
    }
}

void Trigger::UpdateShape()
{
    sf::Vector2f size = mCorner2 - mCorner1;
    sf::Vector2f center = mCorner1 + 0.5f * size ;
	mShape = sf::RectangleShape( size );
	mShape.setFillColor( mColor );
	mShape.setPosition( mCorner1 );
	mShape.setOutlineThickness( 0 );
    if(!mBody)
    {
        b2BodyDef def;
        def.position.Set(center.x, center.y);
        def.userData = this;
        def.type = b2_staticBody;
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
    fixDef.isSensor = mIsSensor; //No collision!
    fixDef.density = 0;
    mFixture = mBody->CreateFixture(&fixDef); // second param is mass, not used here
}

//checks if the player is inside and calls OnLeave()/OnEnter() accordingly
void Trigger::Update(unsigned int deltaT_msec)
{
    bool playerIsInside = false;
    for(const b2ContactEdge* it = mBody->GetContactList(); it != NULL; it = it->next)
    {
        b2Contact* contact = it->contact;
        if(contact->IsTouching())
        {
            Object* other = reinterpret_cast<Object*>(it->other->GetUserData());
            if(other->GetType() == "Player")
            {
                playerIsInside = true;
                break;
            }
        }
    }
    if(playerIsInside != mPlayerWasInside)
    {
        if(mPlayerWasInside)
        {
            OnLeave();
        }
        else
        {
            OnEnter();
        }
    }
    mPlayerWasInside = playerIsInside;
}

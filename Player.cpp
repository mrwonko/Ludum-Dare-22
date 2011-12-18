#include "Player.h"
#include "Level.h"
#include "Helpers.h"
#include <Box2D/Box2D.h>
#include "Sounds.h"

extern Sounds g_Sounds;
extern sf::RenderWindow* g_Window;

Player::Player(Level* const level):
    Object(level),
    mRepresentative(sf::Shape::Rectangle(-Constants::PLAYER_HALFWIDTH, -Constants::PLAYER_HALFHEIGHT, 2 * Constants::PLAYER_HALFWIDTH, 2 * Constants::PLAYER_HALFHEIGHT, Constants::PLAYER_COLOR)),
    mJumpedThisFrame(false)
{
    //ctor
    b2World& world = mLevel->GetWorld();

    b2BodyDef def;
    //def.bullet = true; //player should never ever penetrate walls etc. (this happens in Super Meat Boy when the game stutters for a second) //actually, this is only relevant to dynamic - dynamic - collisions.
    def.type = b2_dynamicBody; //yep, the player can move.
    def.userData = this;
    def.linearDamping = Constants::PLAYER_DAMPING;
    def.fixedRotation = true; //player shall not rotate!
    mBody = world.CreateBody(&def);

    b2PolygonShape shape;
    shape.SetAsBox(Constants::PLAYER_HALFWIDTH, Constants::PLAYER_HALFHEIGHT);

    b2FixtureDef fixDef;
    fixDef.shape = &shape;
    fixDef.friction = Constants::PLAYER_FRICTION;
    fixDef.density = Constants::PLAYER_DENSITY;
    mBody->CreateFixture(&fixDef);

    mJumpChannel.SetBuffer(g_Sounds.Jump);
}

Player::~Player()
{
    //dtor
}

void Player::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    if(!mLevel->IsLost()) //Player explodes on death and is no longer rendered!
    {
        target.Draw(mRepresentative);
    }
}

const bool Player::Serialize(std::ostream& out_stream) const
{
    out_stream << GetPosition().x << " " << GetPosition().y << std::endl;
    return true;
}
const bool Player::Deserialize(std::istream& stream)
{
    sf::Vector2f pos;
    stream >> pos.x >> pos.y;
    if(stream.fail())
    {
        std::cerr << "Could not read player position!" << std::endl;
        return false;
    }
    SetPosition(pos);
    mBody->SetTransform(b2Vec2(pos.x, pos.y), 0);
    mBody->SetLinearVelocity(b2Vec2(0, 0));
    return true;
}

namespace
{
    struct contactInfo
    {
        b2Vec2 contactPoint,
               normal;
    };
    class ContactPointCallback : public b2QueryCallback
    {
    public:
        ContactPointCallback() {}
        typedef std::vector<contactInfo> foo; //may not need all of this
        virtual bool ReportFixture (b2Fixture *fixture)
        {
            return true; //continue
        }

    private:

    };
}

const bool Player::ProcessEvent(const sf::Event& event)
{
    // horizontal movement is handled in Update()
    if(event.Type == sf::Event::KeyPressed && (event.Key.Code == Constants::MOVEU_KEY || event.Key.Code == Constants::JUMP_KEY))
    {
        //TODO: is player on floor? on wall? ...
        if(!mJumpedThisFrame) //only one impulse per frame
        {
            mJumpedThisFrame = true;

            b2Vec2 idealNormal(0, 1);
            b2Vec2 closestNormal(0, -1); //normal from player to contacted thing. worst case: anything is better than this.
            float closestDotProduct = -1; // dot product between the closest normal so far and the ideal normal. higher is better (since 1 means "vectors are equal")
            b2Vec2 contactPoint;
            b2Body* contactor = NULL;
            for(const b2ContactEdge* it = mBody->GetContactList(); it != NULL; it = it->next)
            {
                b2Contact* contact = it->contact;
                if(!contact->IsTouching())
                {
                    continue;
                }
                // get manifold in world coordinates
                b2WorldManifold worldManifold;
                contact->GetWorldManifold(&worldManifold);
                // is this normal better? (i.e. points further downwards - always jump from the most groundlike surface)
                float newDotProduct = b2Dot(idealNormal, worldManifold.normal);;
                if(newDotProduct > closestDotProduct)
                {
                    int pointCount = contact->GetManifold()->pointCount;
                    if(pointCount == 0)
                    {
                        //sensors create contacts without contact points. Skip them.
                        continue;
                    }

                    //set normal, dot product, contactor...
                    closestNormal = worldManifold.normal;
                    closestDotProduct = newDotProduct;
                    contactor = it->other;
                    //... and calculate median contact point.
                    contactPoint = b2Vec2(0, 0);
                    for(int i = 0; i < pointCount; ++i)
                    {
                        contactPoint = contactPoint + worldManifold.points[i];
                    }
                    contactPoint = 1.f/pointCount * contactPoint;
                }
            }

            //cannot jump if no contact with surface
            //cannot jump from surfaces more than 90° far from ideal - I apparently don't even get any contact points from bodies above the player but let's be safe.
            if(contactor != NULL && closestDotProduct >= 0)
            {
                //Impulse depends on direction
                b2Rot angle(acos(closestDotProduct) / 4.f);
                b2Vec2 impulseDir = angle.GetYAxis();
                impulseDir.y = -impulseDir.y;
                if(closestNormal.x < 0)
                {
                    impulseDir.x = -impulseDir.x;
                }
                b2Vec2 impulse = Constants::PLAYER_JUMP_FORCE * impulseDir;
                mBody->ApplyLinearImpulse(impulse, contactPoint);
                //apply opposite impulse to thing you jumped away from (opposing force)
                contactor->ApplyLinearImpulse(-impulse, contactPoint);
                //Play jump sound
                mJumpChannel.Play();
            }
        }
        return true;
    }
    return false;
}

void Player::Update(unsigned int deltaT_msec)
{
    mJumpedThisFrame = false;
    if(mBody->IsAwake())
    {
        const b2Vec2& pos = mBody->GetPosition();
        SetPosition(pos.x, pos.y);
        SetViewPos(*g_Window, sf::Vector2f(pos.x, pos.y));
    }
    //move right
    if(sf::Keyboard::IsKeyPressed(Constants::MOVER_KEY) && mBody->GetLinearVelocity().x < Constants::PLAYER_MAX_VELOCITY)
    {
        mBody->ApplyForce(b2Vec2(Constants::PLAYER_MOVE_FORCE, 0), mBody->GetPosition());
    }
    if(sf::Keyboard::IsKeyPressed(Constants::MOVEL_KEY) && mBody->GetLinearVelocity().x > -Constants::PLAYER_MAX_VELOCITY)
    {
        mBody->ApplyForce(b2Vec2(-Constants::PLAYER_MOVE_FORCE, 0), mBody->GetPosition());
    }
}

#include "Player.h"
#include "Level.h"
#include "Helpers.h"
#include <Box2D/Box2D.h>
#include <sstream>
#include <cmath>
#include <list>
#include "Sounds.h"
#include <SFML/Network/Packet.hpp>
#include <SFML/Graphics.hpp>

extern Sounds g_Sounds;
extern sf::Font g_Font;

Player::Player(Level* const level):
    Object(level),
    mRepresentative(sf::Shape::Circle(0, 0, Constants::PLAYER_RADIUS, Constants::PLAYER_COLOR)),
    mIndex(-1),
    mDead(false),
    mAngleIndicator(sf::Shape::Line(sf::Vector2f(0.f, 0.f), sf::Vector2f(2.f * Constants::PLAYER_RADIUS, 0.f), 0.05f, sf::Color::White)),
    mTargetIndex(-1),
    mAimTime(0)
{
    mIndexText.SetFont(g_Font);
    mIndexText.SetScale(0.05f, 0.05f);
    UpdateIndex();
    //ctor
    b2World& world = mLevel->GetWorld();

    b2BodyDef def;
    def.type = b2_dynamicBody; //yep, the player can move.
    def.userData = this;
    def.linearDamping = Constants::PLAYER_DAMPING;
    def.fixedRotation = true; //player shall not rotate on its own - only on command!
    mBody = world.CreateBody(&def);

    b2CircleShape shape;
    shape.m_radius = Constants::PLAYER_RADIUS;

    b2FixtureDef fixDef;
    fixDef.shape = &shape;
    fixDef.friction = Constants::PLAYER_FRICTION;
    fixDef.density = Constants::PLAYER_DENSITY;
    mBody->CreateFixture(&fixDef);
}

Player::~Player()
{
    //dtor
}

void Player::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    if(!mDead) //Player explodes on death and is no longer rendered!
    {
        target.draw(mRepresentative);
        target.draw(mAngleIndicator);
        target.draw(mIndexText);

        if(mIndex != -1 && mIndex == mLevel->GetIndex())
        {
            //draw orders
        }
    }
}

const bool Player::Serialize(std::ostream& out_stream) const
{
    out_stream << GetPosition().x << " " << GetPosition().y << " " << mIndex << std::endl;
    return !out_stream.fail();
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

    stream >> mIndex;
    UpdateIndex();
    if(stream.fail())
    {
        std::cerr << "Could not read player index!" << std::endl;
        return false;
    }

    return true;
}

const bool Player::Serialize(sf::Packet& out_packet) const
{
    out_packet << GetPosition().x << GetPosition().y << mIndex;
    return bool(out_packet);
}

const bool Player::Deserialize(sf::Packet& packet)
{
    sf::Vector2f pos;
    packet >> pos.x >> pos.y;
    if(!packet)
    {
        std::cerr << "Could not read player position!" << std::endl;
        return false;
    }
    SetPosition(pos);

    int index;
    packet >> index;
    if(!packet)
    {
        std::cerr << "Could not read player index!" << std::endl;
        return false;
    }
    assert(index == mIndex && "Player deserialized from wrong index!");

    return true;
}

const bool Player::ProcessEvent(const sf::Event& event)
{
    return false;
}

void Player::Update(unsigned int deltaT_msec)
{
    if(mDead)
    {
        return;
    }
    //follow target (angle), if any and still visible
    //if not visible anymore: lose it
    if(mTargetIndex != -1)
    {
        Player* target = mLevel->GetPlayer(mTargetIndex);
        if(IsVisible(target) && !target->IsDead())
        {
            sf::Vector2f dif = target->GetPosition() - GetPosition();
            mAngleIndicator.SetRotation(180.f / M_PI * atan2(dif.y, dif.x));

            mAimTime += deltaT_msec;
            if(mAimTime >= Constants::PLAYER_AIM_TIME)
            {
                mLevel->GetParticleSystem().CreateShot(GetPosition(), target->GetPosition());
                target->Kill();
                mTargetIndex = -1;
                mAimTime = 0;
            }
        }
        else
        {
            mAimTime = 0;
            mTargetIndex = -1;
        }
    }
    if(mBody->IsAwake())
    {
        const b2Vec2& pos = mBody->GetPosition();
        sf::Drawable::SetPosition(pos.x, pos.y);
    }
    //  try and aim
    //only if currently no target
    if(mTargetIndex == -1)
    {
        //who's in the 90° cone in front?
        float bestDot = -1.f;

        float myAngleRad = M_PI / 180.f * mAngleIndicator.GetRotation();
        b2Vec2 myAngle(cos(myAngleRad), sin(myAngleRad));
        for(unsigned int i = 0; i < mLevel->GetNumPlayers(); ++i)
        {
            if(i == mIndex) //won't shoot myself
            {
                continue;
            }
            Player* target = mLevel->GetPlayer(i);
            assert(target != NULL);
            sf::Vector2f dif = target->GetPosition() - GetPosition();
            b2Vec2 b2Dif(dif.x, dif.y);
            b2Dif.Normalize();
            assert(b2Dif.LengthSquared() > 0.f);
            float dot = b2Dot(myAngle, b2Dif);
            if(dot < cos(M_PI_2)) //not in front?
            {
                continue;
            }
            if(target->IsDead()) //not alive?
            {
                continue;
            }
            if(!IsVisible(target)) //not visible?
            {
                continue;
            }
            if(dot < bestDot) //not more in front than last one?
            {
                continue;
            }
            mTargetIndex = i;
        }
    }
    //  move towards next waypoint
    //no moving if target acquired
    if(mTargetIndex != -1)
    {
        return;
    }
}

namespace
{
    // This callback finds the closest hit. Given body is ignored
    class RayCastClosestCallback : public b2RayCastCallback
    {
    public:
        RayCastClosestCallback(b2Body* ignoreMe) :
            mHit(false),
            mTarget(NULL),
            mIgnored(ignoreMe)
        {
        }

        float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
            const b2Vec2& normal, float32 fraction)
        {
            b2Body* body = fixture->GetBody();
            if(body == mIgnored)
            {
                //filter
                return -1.f;
            }

            mTarget = body;
            mHit = true;
            mPoint = point;
            mNormal = normal;
            return fraction;
        }

        bool mHit;
        b2Vec2 mPoint;
        b2Vec2 mNormal;
        b2Body* mTarget;

    private:
        b2Body* mIgnored;
    };
}

const bool Player::IsVisible(Player* player)
{
    assert(player != NULL);
    assert(player != this);
    if(player->GetPosition() == GetPosition())
    {
        return true;
    }
    RayCastClosestCallback callback(mBody);
    b2World& world = mLevel->GetWorld();
    world.RayCast(&callback, b2Vec2(GetPosition().x, GetPosition().y), b2Vec2(player->GetPosition().x, player->GetPosition().y));
    if(callback.mHit)
    {
        return player == callback.mTarget->GetUserData();
    }
    return false;
}

void Player::SetPosition(const sf::Vector2f& pos)
{
    sf::Drawable::SetPosition(pos);
    mBody->SetTransform(b2Vec2(pos.x, pos.y), 0);
    mBody->SetLinearVelocity(b2Vec2(0, 0));
}

void Player::Stop()
{
    mBody->SetLinearVelocity(b2Vec2(0, 0));
    mBody->SetAwake(false);
}

void Player::Kill()
{
    mDead = true;
    mLevel->GetParticleSystem().CreateExplosion(GetPosition());
}

void Player::UpdateIndex()
{
    std::stringstream ss;
    ss << mIndex + 1;
    mIndexText.SetString(ss.str());
    sf::FloatRect fr = mIndexText.GetRect();
    mIndexText.SetPosition(-fr.Width/2, -fr.Height/2);
    if(mIndex != -1 && mIndex == mLevel->GetIndex())
    {
        mIndexText.SetColor(sf::Color::Green);
    }
}

const bool Player::SerializeOrders(sf::Packet& out_packet) const
{
    return true;
}

const bool Player::DeserializeOrders(sf::Packet& packet)
{
    return true;
}

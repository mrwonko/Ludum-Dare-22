#include "Player.h"
#include "Level.h"
#include "Helpers.h"
#include <Box2D/Box2D.h>
#include <sstream>
#include "Sounds.h"
#include <SFML/Network/Packet.hpp>
#include <SFML/Graphics.hpp>

extern Sounds g_Sounds;
extern sf::Font g_Font;

Player::Player(Level* const level):
    Object(level),
    mRepresentative(sf::Shape::Circle(0, 0, Constants::PLAYER_RADIUS, Constants::PLAYER_COLOR)),
    mIndex(-1),
    mDead(false)
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
        target.Draw(mRepresentative);
        target.Draw(mIndexText);

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
    return false;
}

void Player::Update(unsigned int deltaT_msec)
{
    if(mBody->IsAwake())
    {
        const b2Vec2& pos = mBody->GetPosition();
        sf::Drawable::SetPosition(pos.x, pos.y);
    }
    // move towards next waypoint
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

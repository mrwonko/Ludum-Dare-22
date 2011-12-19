#include "StaticRect.h"
#include "Constants.h"
#include "Level.h"
#include <Box2D/Box2D.h>
#include <algorithm>
//#include "EventListenerList.h"

//extern EventListenerList g_EventListeners;

std::vector<sf::Color> StaticRect::sColors;

StaticRect::StaticRect(Level* const level) :
    Object(level),
    mColorIndex(0),
    mBody(NULL),
    mFixture(NULL)
{
    //ctor
    if(sColors.empty())
    {
        sColors.push_back(sf::Color::White);
        sColors.push_back(sf::Color::Red);
        sColors.push_back(sf::Color::Yellow);
        sColors.push_back(sf::Color::Blue);
        sColors.push_back(sf::Color::Cyan);
        sColors.push_back(sf::Color::Magenta);
        sColors.push_back(sf::Color(255, 127, 0)); // orange
    }
    mColor = sColors[mColorIndex];
}

StaticRect::~StaticRect()
{
    //dtor
    if(mBody)
    {
        mLevel->GetWorld().DestroyBody(mBody);
        mBody = NULL;
    }
}

const bool StaticRect::Serialize(std::ostream& out_stream) const
{
    out_stream << "\t\t"
               << mCorner1.x << " " << mCorner1.y << " "
               << mCorner2.x<< " "  << mCorner2.y << " "
               << int(mColor.r)<< " "  << int(mColor.g)<< " "  << int(mColor.b) << " " << int(mColor.a) << std::endl;
    return true;
}

const bool StaticRect::Deserialize(std::istream& stream)
{
    stream >> mCorner1.x >> mCorner1.y >> mCorner2.x >> mCorner2.y;
    int temp;
    stream >> temp;
    mColor.r = temp;
    stream >> temp;
    mColor.g = temp;
    stream >> temp;
    mColor.b = temp;
    stream >> temp;
    mColor.a = temp;
    if(stream.fail())
    {
        std::cerr << "Invalid StaticRect format!" << std::endl;
        return false;
    }
    UpdateShape();
    return true;
}

void StaticRect::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    target.Draw(mShape);
}

void StaticRect::SetCorner1(const sf::Vector2f& pos)
{
    mCorner1 = pos;
    UpdateShape();
}

void StaticRect::SetCorner2(const sf::Vector2f& pos)
{
    mCorner2 = pos;
    UpdateShape(); //unnecessarily updates physics body as well... could care less *shrug*
}

void StaticRect::SetColor(const sf::Color& color)
{
    mColor = color;
    UpdateShape();
}

void StaticRect::UpdateShape()
{
    sf::Vector2f size = mCorner2 - mCorner1;
    sf::Vector2f center = mCorner1 + 0.5f * size ;
    mShape = sf::Shape::Rectangle(-size.x/2, -size.y/2, size.x, size.y, mColor, Constants::STATICRECT_BORDERSIZE, mColor);
    mShape.SetPosition(center);
    mShape.EnableFill(false);

    CreateBody(b2Vec2(center.x, center.y));
    CreateFixture(std::abs(size.x/2), std::abs(size.y/2));
}

void StaticRect::CreateBody(const b2Vec2& pos)
{
    if(!mBody)
    {
        b2BodyDef def;
        def.position = pos;
        def.userData = this;
        mBody = mLevel->GetWorld().CreateBody(&def);
    }
    else
    {
        mBody->SetTransform(pos, 0);
    }
}

void StaticRect::CreateFixture(const float hsizeX, const float hsizeY)
{
    if(mFixture)
    {
        mBody->DestroyFixture(mFixture);
    }
    b2PolygonShape shape;
    shape.SetAsBox(hsizeX, hsizeY);
    mFixture = mBody->CreateFixture(&shape, 0.f); // second param is mass, not used here
}

void StaticRect::Edit_OnClicked(const sf::Mouse::Button button)
{
    //cycle color
    if(button == sf::Mouse::Left)
    {
        ++mColorIndex;
        if(mColorIndex >= sColors.size()) //shouldn't be greater but better safe than sorry
        {
            mColorIndex = 0;
        }
        SetColor(sColors[mColorIndex]);
    }
    else if(button == sf::Mouse::Right)
    {
        if(mColorIndex == 0) //cannot be smaller, is unsigned
        {
            mColorIndex = sColors.size();
        }
        --mColorIndex;
        SetColor(sColors[mColorIndex]);
    }
}

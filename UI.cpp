#include "UI.h"
#include "UIElement.h"

UI::UI()
{
    //ctor
}

UI::~UI()
{
    for(ElementMap::const_iterator it = mElements.begin(); it != mElements.end(); ++it)
    {
        delete (it->second);
    }
}

/** Element will henceforth be managed by the UI, i.e. will be deleted when the UI is deleted. Get it via name. Any previous item of the same name will be deleted. **/
void UI::AddElement(const std::string& name, UIElement* element)
{
    if(mElements.find(name) != mElements.end())
    {
        delete mElements[name];
    }
    mElements[name] = element;
}

/** Returns NULL if no such UIElement exists. **/
UIElement* UI::GetElement(const std::string& name)
{
    ElementMap::iterator it = mElements.find(name);
    if(it == mElements.end())
    {
        return NULL;
    }
    return it->second;
}

namespace
{
    sf::Vector2f CalculateActualPosition(const sf::Vector2f& dimensions, sf::Vector2f coordinate) //note the call by value - I'm modifying & returning a copy
    {
        if(coordinate.x < 0)
        {
            coordinate.x += dimensions.x;
        }
        if(coordinate.y < 0)
        {
            coordinate.y += dimensions.y;
        }
        return coordinate;
    }
}

void UI::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    //save old view (gamefield) and create new one (UI space)
    sf::View oldView = target.GetView();
    sf::Vector2f dimensions(target.GetWidth(), target.GetHeight());
    sf::View view(dimensions * 0.5f, dimensions);
    target.SetView(view);

    //render elements, calculating their actual positions (i.e. converting negative coordinates)
    for(ElementMap::const_iterator it = mElements.begin(); it != mElements.end(); ++it)
    {
        UIElement& element = *(it->second);
        element.SetPosition(CalculateActualPosition(dimensions, element.GetCoordinates()));
        target.Draw(element);
    }

    //revert old view
    target.SetView(oldView);
}

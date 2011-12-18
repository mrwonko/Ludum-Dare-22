#include "UIText.h"
#include <SFML/Graphics/RenderTarget.hpp>

extern sf::Font g_Font;

UIText::UIText()
{
    mText.SetFont(g_Font);
    //default character size is 30
    //ctor
}

UIText::~UIText()
{
    //dtor
}

void UIText::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
    target.Draw(mText);
}

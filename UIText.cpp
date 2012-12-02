#include "UIText.h"
#include <SFML/Graphics/RenderTarget.hpp>

extern sf::Font g_Font;

UIText::UIText()
{
    mText.setFont(g_Font);
    //default character size is 30
    //ctor
}

UIText::~UIText()
{
    //dtor
}

void UIText::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
    target.draw(mText, states);
}

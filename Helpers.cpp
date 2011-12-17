#include <SFML/Graphics.hpp>
#include "Helpers.h"
#include "Constants.h"

extern sf::RenderWindow g_Window;

void SetViewPos(const sf::Vector2f& center)
{
    //I could cache this by making it static but the viewsize might change?
    sf::Vector2f viewSize(float(g_Window.GetWidth()) / g_Window.GetHeight() * GAMEFIELD_HEIGHT, GAMEFIELD_HEIGHT);
    sf::View view(center, viewSize);
    g_Window.SetView(view);
}

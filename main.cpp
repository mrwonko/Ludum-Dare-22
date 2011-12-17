#include <SFML/Graphics.hpp>
#include "Helpers.h"

sf::RenderWindow g_Window(sf::VideoMode(800, 600), "Mr. Wonkos Ludum Dare 22 Game"); //default style -> close & resize

int main()
{
    while(g_Window.IsOpened())
    {
        sf::Event ev;
        while(g_Window.PollEvent(ev))
        {
            switch(ev.Type)
            {
            case sf::Event::Closed:
                {
                    g_Window.Close();
                    break;
                }
            case sf::Event::Resized:
                {
                    //update window view, keeping old center but possibly changing the size (i.e. update aspect accordingly)
                    SetViewPos(g_Window.GetView().GetCenter());
                }
            case sf::Event::KeyPressed:
                {
                    // insert key code here
                    break;
                }
            default:
                {
                    break;
                }
            }
        }
        g_Window.Clear();

        // render

        g_Window.Display();
    }
    return 0;
}

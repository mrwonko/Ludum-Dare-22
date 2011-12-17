#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow App(sf::VideoMode(800, 600), "Mr. Wonkos Ludum Dare 22 Game", sf::Style::Close);
    while(App.IsOpened())
    {
        sf::Event ev;
        while(App.PollEvent(ev))
        {
            switch(ev.Type)
            {
            case sf::Event::Closed:
                {
                    App.Close();
                    break;
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
        App.Clear();

        // render

        App.Display();
    }
    return 0;
}

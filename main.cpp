#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Helpers.h"
#include "Level.h"
#include "EventListenerList.h"
#include "Textures.h"
#include "Sounds.h"

EventListenerList g_EventListeners;
sf::Font g_Font;
sf::RenderWindow* g_Window = NULL;
Textures g_Textures;
Sounds g_Sounds;

namespace
{
    void SetWinView(sf::RenderTarget& target)
    {
        float height = 600;
        float width = height * target.getSize().x / target.getSize().y;
        target.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(width, height)));
    }
}

int main()
{
    if(!g_Font.loadFromFile(Constants::FONTPATH))
    {
        std::cerr << "Could not load font \"" << Constants::FONTPATH << "\"!" << std::endl;
        return 0;
    }
    if(!g_Textures.Load())
    {
        std::cerr << "Could not load textures!" << std::endl;
        return 0;
    }
    if(!g_Sounds.Load())
    {
        std::cerr << "Could not load sounds!" << std::endl;
        return 0;
    }
    int numLevels = GetLevelCount();
    std::cout << "Found " << numLevels << " levels." << std::endl;

    if(numLevels <= 0)
    {
        std::cerr << "No levels found!" << std::endl;
        return 0;
    }
    int curLevelIndex = -1; // when this == numLevels, we're at the game won screen.

    sf::RenderWindow window(sf::VideoMode(800, 600), "Mini Blue Box Boy"); //default style -> close & resize
    window.setKeyRepeatEnabled(false); // no holding the jump button
    g_Window = &window;

    Level* curLevel = NULL;

    SetViewPos(window, sf::Vector2f(0, 0));

    sf::Text winnerText;
    winnerText.setFont(g_Font);
    winnerText.setString("You win!");
    winnerText.setCharacterSize(200.f);
	sf::FloatRect textRec = winnerText.getGlobalBounds();
    winnerText.setPosition(-textRec.width / 2.f, -textRec.height / 2.f);
    winnerText.setColor(sf::Color::White);

	sf::Clock clock;
	sf::Int32 lastFrametime = 0;

    bool inFocus = true;
    while(window.isOpen())
    {
        //  Event Handling
        sf::Event ev;
        while(window.pollEvent(ev))
        {
            switch(ev.type)
            {
            case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
            case sf::Event::Resized:
                {
                    if(curLevelIndex < numLevels)
                    {
                        //update window view, keeping old center but possibly changing the size (i.e. update aspect accordingly)
                        SetViewPos(window, window.getView().getCenter());
                    }
                    else
                    {
                        SetWinView(window);
                    }
                    break;
                }
            case sf::Event::LostFocus:
                {
                    inFocus = false;
                    break;
                }
            case sf::Event::GainedFocus:
                {
                    inFocus = true;
                    break;
                }
            case sf::Event::KeyPressed:
                {
                    if(ev.key.code == Constants::PREVLEVEL_KEY)
                    {
                        if(curLevelIndex > 0)
                        {
                            if(curLevelIndex < numLevels)
                            {
                                delete curLevel;
                                g_EventListeners.Clear();
                            }
                            --curLevelIndex;
                            curLevel = new Level(curLevelIndex);
                            if(!curLevel->Load())
                            {
                                window.close();
                            }
                        }
                    }
                    else if(ev.key.code == Constants::NEXTLEVEL_KEY)
                    {
                        if(curLevelIndex < numLevels)
                        {
                            delete curLevel;
                            curLevel = NULL;
                            g_EventListeners.Clear();
                            ++curLevelIndex;
                            if(curLevelIndex < numLevels)
                            {
                                curLevel = new Level(curLevelIndex);
                                if(!curLevel->Load())
                                {
                                    window.close();
                                }
                            }
                            else
                            {
                                SetWinView(window);
                            }
                        }
                    }
                    else if (ev.key.code == sf::Keyboard::F6) //can't be arsed to add constant
                    {
                        if(curLevelIndex < numLevels)
                        {
                            delete curLevel;
                            g_EventListeners.Clear();
                        }
                        curLevelIndex = numLevels;
                        ++numLevels;
                        curLevel = new Level(curLevelIndex);
                    }
                    else
                    {
                        g_EventListeners.ProcessEvent(ev);
                    }
                    break;
                }
            default:
                {
                    g_EventListeners.ProcessEvent(ev);
                    break;
                }
            }
        }

        //  Level changing
        assert(curLevelIndex >= -1);
        if(curLevelIndex == -1 || curLevel->IsComplete())
        {
            if(curLevel != NULL)
            {
                delete curLevel;
                curLevel = NULL;
                g_EventListeners.Clear(); // there shouldn't be any event listeners that are not children of the level, so let's clean up to be sure we're not accessing dead pointers
            }
            ++curLevelIndex;
            if(curLevelIndex < numLevels)
            {
                curLevel = new Level(curLevelIndex);
                if(!curLevel->Load())
                {
                    window.close();
                }
            }
            else
            {
                SetWinView(window);
            }
        }

        //  Game Logic
		sf::Int32 frametime = clock.getElapsedTime().asMilliseconds();
		sf::Int32 deltaT = frametime - lastFrametime;
		deltaT = std::min(deltaT, sf::Int32(66)); //less than 15 fps may be bad.
		lastFrametime = frametime;
        if(inFocus) //no update when inactive
        {
            if(curLevel != NULL)
            {
                curLevel->Update(deltaT);
            }
        }
        else
        {
            sf::sleep(sf::seconds(5)); //also, don't clog the cpu
        }

        //  Rendering

        window.clear();
        if(curLevelIndex < numLevels)
        {
            window.draw(*curLevel);
        }
        else
        {
            window.draw(winnerText);
        }

        window.display();
    }
    if(curLevel != NULL)
    {
        g_EventListeners.Remove(curLevel);
        delete curLevel;
    }
    g_Window = NULL;
    return 0;
}

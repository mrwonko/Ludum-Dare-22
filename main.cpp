#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Helpers.h"
#include "Level.h"
#include "EventListenerList.h"

EventListenerList g_EventListeners;
sf::Font g_Font;
sf::RenderWindow* g_Window = NULL;

int main()
{
    if(!g_Font.LoadFromFile(Constants::FONTPATH))
    {
        std::cerr << "Could not load font \"" << Constants::FONTPATH << "\"!" << std::endl;
        return 0;
    }
    int numLevels = GetLevelCount();
    std::cout << "Found " << numLevels << " levels." << std::endl;

    if(numLevels <= 0)
    {
        std::cerr << "No levels found!" << std::endl;
        return 0;
    }
    int curLevelIndex = -1;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Mr. Wonkos Ludum Dare 22 Game"); //default style -> close & resize
    g_Window = &window;

    Level* curLevel = NULL;

    /*
    b2Vec2 gravity(0.f, 10.f);
    b2World world(gravity);
    world.SetAllowSleeping(true);
    { // ground body definition - static by default
        b2BodyDef def;
        def.position.Set(0.f, 10.f);
        b2Body* body = world.CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(40.f, 5.f);
        body->CreateFixture(&shape, 0.f); // second param is mass, not used here
    }
    {
        b2BodyDef def;
        def.position.Set(0.f, 0.f);
        def.type = b2_dynamicBody; // override static body default
        b2Body* body = world.CreateBody(&def);
        b2PolygonShape shape;
        shape.SetAsBox(2.f, 2.f);
        b2FixtureDef fdef;
        fdef.shape = &shape;
        fdef.density = 1.f; //used to calculate mass
        fdef.friction = 0.3f;
        body->CreateFixture(&fdef); //no mass means: calculate
    }
    {
        b2BodyDef def;
        def.position.Set(2.1f, -5.f);
        def.type = b2_dynamicBody; // override static body default
        b2Body* body = world.CreateBody(&def);
        b2CircleShape shape;
        shape.m_radius = 2.f;
        b2FixtureDef fdef;
        fdef.shape = &shape;
        fdef.density = 1.f; //used to calculate mass
        fdef.friction = 0.5f;
        body->CreateFixture(&fdef); //no mass means: calculate
    }

    sfmlBox2DDebugDraw debugDraw;
    debugDraw.SetWorld(&world);
    */

    SetViewPos(window, sf::Vector2f(0, 0));


    while(window.IsOpened())
    {
        //  Event Handling
        sf::Event ev;
        while(window.PollEvent(ev))
        {
            switch(ev.Type)
            {
            case sf::Event::Closed:
                {
                    window.Close();
                    break;
                }
            case sf::Event::Resized:
                {
                    //update window view, keeping old center but possibly changing the size (i.e. update aspect accordingly)
                    SetViewPos(window, window.GetView().GetCenter());
                }
            default:
                {
                    g_EventListeners.ProcessEvent(ev);
                }
            }
        }

        //  Level changing
        if(curLevel == NULL or curLevel->IsComplete())
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
                    window.Close();
                    break;
                }
            }
            else
            {
                window.Close();
                break;
            }
        }

        assert(curLevel != NULL);

        //  Game Logic
        unsigned int frametime = std::min(window.GetFrameTime(), sf::Uint32(66)); //less than 15 fps may be bad.
        curLevel->Update(frametime);

        //  Rendering

        window.Clear();
        window.Draw(*curLevel);

        window.Display();
    }
    if(curLevel != NULL)
    {
        g_EventListeners.Remove(curLevel);
        delete curLevel;
    }
    g_Window = NULL;
    return 0;
}

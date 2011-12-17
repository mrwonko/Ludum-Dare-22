#include <SFML/Graphics.hpp>
#include "Helpers.h"

#include <Box2D/Box2D.h>
#include "sfmlBox2DDebugDraw.h"

sf::RenderWindow g_Window(sf::VideoMode(800, 600), "Mr. Wonkos Ludum Dare 22 Game"); //default style -> close & resize

int main()
{
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
    bool debugPhysics = false;

    SetViewPos(sf::Vector2f(0, 0));


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
                    switch(ev.Key.Code)
                    {
                    case sf::Keyboard::P:
                        {
                            debugPhysics = !debugPhysics;
                        }
                    default:
                        {
                            break;
                        }
                    }
                }
            default:
                {
                    break;
                }
            }
        }
        g_Window.Clear();

        // render

        if(debugPhysics)
        {
            g_Window.Draw(debugDraw);
        }

        static const int32 velocityIterations = 6;
        static const int32 positionIterations = 2;
        world.Step(g_Window.GetFrameTime() / 1000.f, velocityIterations, positionIterations);

        g_Window.Display();
    }
    return 0;
}

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <algorithm>
#include <sstream>
#include <vector>
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
        float width = height * target.GetWidth() / target.GetHeight();
        target.SetView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(width, height)));
    }

    enum NetMessages
    {
        eDisconnect, //clients informing server of disconnect
        eShutdown, //server informing clients of shutdown
        eLevel, //server sending level to clients
        ePlayer, //server sending player information to clients or vice versa
        eIndex, //server telling client his index
        eFull, //server telling client he's full
        eWelcome, //server telling client: welcome!
        eStart, //server: game starts
        eOutcome, //server: show outcome
        eReady, //player is ready
    };

    typedef std::list<sf::TcpSocket*> SocketVector;

    void SendPacket(SocketVector& targets, sf::Packet& packet)
    {
        for(SocketVector::iterator it = targets.begin(); it != targets.end(); ++it)
        {
            sf::TcpSocket* socket = *it;
            socket->Send(packet);
        }
    }

    void SendQuitMessage(SocketVector& clients)
    {
        sf::Packet packet;
        packet << eShutdown;
        SendPacket(clients, packet);
    }

    void Delete(SocketVector& killus)
    {
        for(SocketVector::iterator it = killus.begin(); it != killus.end(); ++it)
        {
            sf::TcpSocket* socket = *it;
            socket->Disconnect();
            delete socket;
        }
        killus.clear();
    }

    struct Client
    {
        unsigned int Index;
        sf::TcpSocket* Socket;
    };
}

int main()
{
    if(!g_Font.LoadFromFile(Constants::FONTPATH))
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
    while(true)
    {
        std::cout<<"\nWhat would you like to do?\n0 Exit\n1 Host Game (dedicated)\n2 Join Game\n3 Edit Level\n\nchoice: " << std::flush;

        bool first = true;
        unsigned int choice = 0;
        do
        {
            if(!first)
            {
                std::cerr<<"Invalid input!\nchoice: "<<std::flush;
            }
            std::cin >> choice;
            first = false;
        }
        while(std::cin.fail() || choice > 3);
        // exit
        if(choice == 0)
        {
            break;
        }

        //host
        if(choice == 1)
        {
            Level level;
            bool loaded = false;
            do
            {
                std::string mapname = AskForMapName();
                level.SetFilename(mapname);
                if(level.Load())
                {
                    loaded = true;
                }
                else
                {
                    std::cout<<"Failed to load "<<mapname<<std::endl;
                }
            }
            while(!loaded);

            SocketVector clientSockets;
            unsigned int numClients = 0;
            unsigned int maxClients = level.GetNumPlayers();
            sf::TcpListener listener;

            listener.SetBlocking(false);
            if(listener.Listen(Constants::PORT) != sf::Socket::Done)
            {
                std::cerr<<"Could not start listening!"<<std::endl;
                continue;
            }

            //wait for clients
            {
                sf::RenderWindow serverWindow(sf::VideoMode(800, 600), "Dedicated Server - waiting for players");
                sf::Text infoText;
                infoText.SetFont(g_Font);
                infoText.SetString("Press Enter to start\nPress Escape/close the window to Cancel");
                sf::Text playerCount;
                playerCount.SetFont(g_Font);
                playerCount.SetString("Players: 0");
                playerCount.SetPosition(0, 300);
                sf::TcpSocket* newSocket = new sf::TcpSocket;
                while(serverWindow.IsOpened())
                {
                    sf::Event ev;
                    while(serverWindow.PollEvent(ev))
                    {
                        //abort
                        if(ev.Type == sf::Event::Closed || (ev.Type == sf::Event::KeyPressed && ev.Key.Code == sf::Keyboard::Escape))
                        {
                            std::cout<<"Server aborting"<<std::endl;
                            serverWindow.Close();
                            SendQuitMessage(clientSockets);
                            Delete(clientSockets); //also clears the vector
                            break;
                        }
                        //start
                        else if(ev.Type == sf::Event::KeyPressed && ev.Key.Code == sf::Keyboard::Return)
                        {
                            std::cout<<"Server starting"<<std::endl;
                            serverWindow.Close();
                            break;
                        }
                        //nothing else to do, right?
                    }
                    //check for new clients
                    if(listener.Accept(*newSocket) == sf::Socket::Done)
                    {
                        std::cout<<newSocket->GetRemoteAddress().ToString() << " connected." <<std::endl;
                        if(numClients < maxClients)
                        {
                            //send welcome packet
                            {
                                sf::Packet packet;
                                packet << eWelcome;
                                newSocket->Send(packet);
                            }
                            //send index packet
                            {
                                sf::Packet packet;
                                packet << eIndex;
                                packet << numClients;
                                newSocket->Send(packet);
                            }
                            clientSockets.push_back(newSocket);
                            ++numClients;
                        }
                        else
                        {
                            //send f*ck off packet
                            sf::Packet packet;
                            packet << eFull;
                            newSocket->Send(packet);
                            delete newSocket;
                        }
                        newSocket = new sf::TcpSocket;

                    }
                    //display client count
                    std::stringstream ss;
                    ss << "Players: " << numClients;
                    playerCount.SetString(ss.str());

                    serverWindow.Clear();
                    serverWindow.Draw(infoText);
                    serverWindow.Draw(playerCount);
                    serverWindow.Display();
                }
                delete newSocket;
            }
            if(clientSockets.empty())
            {
                std::cout<< "Server is done."<<std::endl;
            }
            else
            {
                //actual server stuff
                std::cout<<"Game starting"<<std::endl;
                //start packet
                {
                    sf::Packet packet;
                    packet << eStart;
                    SendPacket(clientSockets, packet);
                }

                assert(numClients == clientSockets.size());

                level.SetNumPlayers(numClients);

                std::cout<<"Sending level to clients"<<std::endl;
                {
                    sf::Packet packet;
                    packet << eLevel;
                    if(!level.Serialize(packet))
                    {
                        std::cerr<<"Could not prepare level for sending to clients!"<<std::endl;
                        continue;
                    }
                    SendPacket(clientSockets, packet);
                }

                unsigned int numReadyClients = 0;

                bool forceQuit = false;

                while(!level.IsOver() && !forceQuit)
                {
                    for(SocketVector::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it)
                    {
                        sf::TcpSocket* socket = *it;
                        socket->SetBlocking(false);
                        sf::Packet packet;
                        //got any packets?
                        if(socket->Receive(packet) == sf::Socket::Done)
                        {
                            sf::Uint32 msg;
                            packet >> msg;
                            if(!packet)
                            {
                                std::cout<<"Error: invalid packet received!"<<std::endl;
                                //let's not quit, prevents abuse - wtf, I'm talking about a lan party here!
                            }
                            else
                            {
                                if(msg == eDisconnect) //cannot handle if he's ready or not, would thus lead to bugs/exploits -> screw it, just quit.
                                {
                                    unsigned int index;
                                    packet >> index;
                                    std::cout<<"Client "<<index<<" disconnected, shutting down (this would be too hard to handle)"<<std::endl;
                                    forceQuit = true;
                                    break;
                                }
                                else if(msg == ePlayer)
                                {
                                    unsigned int index;
                                    packet >> index;
                                    Player* player = level.GetPlayer(index);
                                    if(player != NULL)
                                    {
                                        assert(player->DeserializeOrders(packet));
                                    }
                                }
                                else if(msg == eReady)
                                {
                                    std::cout<<"A player is ready! " <<numReadyClients << "/" << clientSockets.size() << std::endl;
                                    ++numReadyClients;
                                    if(numReadyClients == clientSockets.size()) //all players ready?
                                    {
                                        numReadyClients = 0;
                                        for(unsigned int i = 0; i < level.GetNumPlayers(); ++i)
                                        {
                                            Player* player = level.GetPlayer(i);
                                            assert(player != NULL);
                                            assert(player->GetIndex() == i);
                                            sf::Packet orderPacket;
                                            orderPacket << ePlayer << i;
                                            assert(player->SerializeOrders(orderPacket));
                                            SendPacket(clientSockets, orderPacket);
                                        }

                                        sf::Packet gogogo;
                                        gogogo << eOutcome;
                                        SendPacket(clientSockets, gogogo);
                                        level.CalculateOutcome();
                                    }
                                }
                                else
                                {
                                    std::cout<<"Received invalid message " << msg << std::endl;
                                }
                            }
                        }
                    }
                    sf::Sleep(50);
                }


                std::cout<<"Server shutting down" << std::endl;
                SendQuitMessage(clientSockets);
                Delete(clientSockets);
            }
//            while(true)
//            {
//                std::cout<< "Accept: " << listener.Accept(*newSocket) << std::endl;
//                if(socket.GetRemoteAddress() != sf::IpAddress::None)
//                {
//                    std::cout<<"Accepted " << socket.GetRemoteAddress().ToString() << std::endl;;
//                    return 0;
//                }
//            }
        }

        //join
        else if(choice == 2)
        {
            std::string strAddress;
            std::cout<<"Host's Address: " << std::flush;
            std::cin.get(); //there's apparently still something in the buffer at this point
            std::getline(std::cin, strAddress);
            sf::IpAddress address(strAddress);
            std::cout<<"Connecting to " << address.ToString() << std::endl;
            sf::TcpSocket socket;
            if(socket.Connect(address, Constants::PORT) == sf::Socket::Done)
            {
                std::cout<<"Connected! Don't abort now, that would be completely unexpected. Seriously. It will break shit."<<std::endl;
            }
            else
            {
                std::cout<<"Could not connect!"<<std::endl;
                std::cin>>choice;
                continue;
            }
            sf::Packet packet;
            if(socket.Receive(packet) != sf::Socket::Done)
            {
                std::cout<<"Error receiving packet!"<<std::endl;
                continue;
            }
            sf::Uint32 msg;
            packet >> msg;
            if(!packet)
            {
                std::cout<<"Invalid packet! Server will now break."<<std::endl;
                continue;
            }
            if(msg == eWelcome)
            {
                std::cout<<"We are welcome!"<<std::endl;
            }
            else if(msg == eFull)
            {
                std::cout<<"Server is full!"<<std::endl;
                continue;
            }
            else
            {
                std::cout<<"Error: Unexpected message! Server will now break." << msg << std::endl;
                continue;
            }

            if(socket.Receive(packet) != sf::Socket::Done)
            {
                std::cout<<"Error receiving packet! Server will now break."<<std::endl;
                continue;
            }
            packet >> msg;
            if(!packet)
            {
                std::cout<<"Invalid packet! Server will now break."<<std::endl;
                continue;
            }
            if(msg != eIndex)
            {
                std::cout<<"Unexpected packet! Server will now break."<<std::endl;
                continue;
            }
            unsigned int playerIndex;
            packet >> playerIndex;
            if(!packet)
            {
                std::cout<<"Invalid packet! Could not read index from index package. Server will now break."<<std::endl;
                continue;
            }
            std::cout<<"We are client " << playerIndex << std::endl;;

            std::cout<<"Waiting for game to start. Still no disconnecting! If you give me less than 1 day to code this, don't expect miracles."<<std::endl;

            if(socket.Receive(packet) != sf::Socket::Done)
            {
                std::cout<<"Error receiving packet! Server will now break."<<std::endl;
                continue;
            }
            packet >> msg;
            if(!packet)
            {
                std::cout<<"Invalid packet! Server will now break."<<std::endl;
                continue;
            }
            if(msg == eShutdown)
            {
                std::cout<<"Server shutting down."<<std::endl;
                continue;
            }
            else if(msg != eStart)
            {
                std::cout<<"Unexpected packet! Server will now break."<<std::endl;
                continue;
            }
            std::cout<<"Game is starting! Waiting for level to be transfered..."<<std::endl;

            if(socket.Receive(packet) != sf::Socket::Done)
            {
                std::cout<<"Error receiving packet! Server will now break."<<std::endl;
                continue;
            }
            packet >> msg;
            if(!packet)
            {
                std::cout<<"Invalid packet! Server will now break."<<std::endl;
                continue;
            }
            if(msg != eLevel)
            {
                std::cout<<"Unexpected packet! Server will now break."<<std::endl;
                continue;
            }
            Level level;
            level.SetIndex(playerIndex);
            if(!level.Deserialize(packet))
            {
                std::cout<<"Invalid packet! Server will now break."<<std::endl;
                continue;
            }
            std::cout<<"Received level. Ready to rumble."<<std::endl;

            sf::RenderWindow wnd(sf::VideoMode(800, 600), "Mini Synapse - Editor");
            g_Window = &wnd;

            sf::Text serverDeadText;
            serverDeadText.SetFont(g_Font);
            serverDeadText.SetString("Server is down.");
            serverDeadText.SetScale(0.05f, 0.05f);
            serverDeadText.SetColor(sf::Color::Red);

            bool serverDead = false;

            while(wnd.IsOpened())
            {
                sf::Event ev;
                while(wnd.PollEvent(ev))
                {
                    if(ev.Type == sf::Event::Closed)
                    {
                        wnd.Close();
                        sf::Packet quitPacket;
                        packet << eDisconnect;
                        packet << playerIndex;
                        socket.SetBlocking(true);
                        socket.Send(packet);
                        break;
                    }
                    else if(ev.Type == sf::Event::Resized)
                    {
                        //update window view, keeping old center but possibly changing the size (i.e. update aspect accordingly)
                        SetViewPos(wnd, wnd.GetView().GetCenter());
                    }
                    else
                    {
                        level.ProcessEvent(ev);
                    }
                }
                socket.SetBlocking(false);
                if(!serverDead && socket.Receive(packet) == sf::Socket::Done)
                {
                    packet >> msg;
                    if(!packet)
                    {
                        std::cerr<<"Error: invalid packet received!"<<std::endl;
                        wnd.Close();
                        break;
                    }
                    else
                    {
                        if(msg == eShutdown)
                        {
                            std::cout<<"Server shutting down." <<std::endl;
                            serverDead = true; //stay open so players can look at outcome - HACK!
                            //wnd.Close();
                            //break;
                        }
                        else if(msg == ePlayer)
                        {
                            int index;
                            packet >> index;
                            if(!packet)
                            {
                                std::cout<<"Error: invalid packet received!"<<std::endl;
                                wnd.Close();
                                break;
                            }
                            Player* player = level.GetPlayer(index);
                            assert(player != NULL);
                            if(!player->DeserializeOrders(packet))
                            {
                                std::cout<<"Error: invalid packet received!"<<std::endl;
                                wnd.Close();
                                break;
                            }

                        }
                        else if(msg == eOutcome)
                        {
                            level.ShowOutcome();
                        }
                        else if(msg == eDisconnect)
                        {
                            int index;
                            packet >> index;
                            assert(bool(packet));
                            Player* player = level.GetPlayer(index);
                            if(player != NULL)
                            {
                                player->Kill();
                            }
                        }
                    }
                }
                if(!serverDead && level.Ready)
                {
                    level.Ready = false;

                    sf::Packet playerPacket;
                    playerPacket << ePlayer;
                    playerPacket << playerIndex;
                    Player* player = level.GetPlayer(playerIndex);
                    assert(player != NULL);
                    assert(player->SerializeOrders(playerPacket));
                    socket.SetBlocking(true);
                    socket.Send(playerPacket);

                    sf::Packet readyPacket;
                    readyPacket << eReady;
                    socket.Send(readyPacket);
                }

                level.Update(wnd.GetFrameTime());

                wnd.Clear();
                wnd.Draw(level);
                if(serverDead)
                {
                    wnd.Draw(serverDeadText);
                }
                wnd.Display();
            }

            g_Window = NULL;
        }
        //editor
        else if(choice == 3)
        {
            Level level(true);
            level.SetFilename(AskForMapName());
            level.Load(); //try loading, accept failure (= new map)

            sf::RenderWindow wnd(sf::VideoMode(800, 600), "Mini Synapse - Editor");
            g_Window = &wnd;

            while(wnd.IsOpened())
            {
                sf::Event ev;
                while(wnd.PollEvent(ev))
                {
                    if(ev.Type == sf::Event::Closed)
                    {
                        wnd.Close();
                        break;
                    }
                    else if(ev.Type == sf::Event::Resized)
                    {
                        //update window view, keeping old center but possibly changing the size (i.e. update aspect accordingly)
                        SetViewPos(wnd, wnd.GetView().GetCenter());
                    }
                    else
                    {
                        level.ProcessEvent(ev);
                    }
                }

                level.Update(wnd.GetFrameTime());

                wnd.Clear();
                wnd.Draw(level);
                wnd.Display();
            }

            g_Window = NULL;

            std::cout<<"TODO: Level editor (vital!)"<<std::endl;
        }
        else
        {
            std::cerr<<"Invalid choice - wtf this shouldn't happen!"<<std::endl;
            continue;
        }
    }
    return 0;
}

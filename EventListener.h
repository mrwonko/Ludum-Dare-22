#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

namespace sf
{
    class Event;
}

class EventListener
{
    public:
        virtual ~EventListener();

        /// \return Whether the event was used (if it's in an EventListenerList and not used, the next one will get the chance to use it)
        virtual const bool ProcessEvent(const sf::Event& event) = 0;

    protected:
        EventListener();
};

#endif // EVENTLISTENER_H

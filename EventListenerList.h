#ifndef EVENTLISTENERSTACK_H
#define EVENTLISTENERSTACK_H

#include <list>

class EventListener;
namespace sf
{
    class Event;
}

/// List of EventListeners to send events to - once somebody used it, alter entries won't get it.
class EventListenerList
{
    public:
        EventListenerList();
        virtual ~EventListenerList();

        void PushBack(EventListener* listener);
        void PushFront(EventListener* listener);
        void Remove(EventListener* listener);

        /// \return Whether the event was used
        const bool ProcessEvent(const sf::Event& event);

    private:
        std::list<EventListener*> mListeners;
};

#endif // EVENTLISTENERSTACK_H

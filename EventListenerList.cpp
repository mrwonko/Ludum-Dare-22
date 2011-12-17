#include "EventListenerList.h"
#include "EventListener.h"

EventListenerList::EventListenerList()
{
    //ctor
}

EventListenerList::~EventListenerList()
{
    //dtor
}

void EventListenerList::PushBack(EventListener* listener)
{
    mListeners.push_back(listener);
}

void EventListenerList::PushFront(EventListener* listener)
{
    mListeners.push_front(listener);
}

void EventListenerList::Remove(EventListener* listener)
{
    for(std::list<EventListener*>::iterator it = mListeners.begin(); it != mListeners.end();)
    {
        if((*it) == listener)
        {
            it = mListeners.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

const bool EventListenerList::ProcessEvent(const sf::Event& event)
{
    //everybody gets a chance to use the event, first one wins.
    for(std::list<EventListener*>::iterator it = mListeners.begin(); it != mListeners.end(); ++it)
    {
        if((*it)->ProcessEvent(event))
        {
            return true;
        }
    }
    return false;
}

void EventListenerList::Clear()
{
    mListeners.clear();
}

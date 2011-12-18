#ifndef EDITACTION_H
#define EDITACTION_H

#include <string>

class Level;
namespace sf
{
    class Event;
}

class EditAction
{
    public:
        virtual ~EditAction() {}

        virtual const bool ProcessEvent(const sf::Event& event) = 0;
        virtual void OnExit() {} //EditAction's changed to another one
        virtual std::string GetName() const = 0;

    protected:
        EditAction(Level* const level) : mLevel(level) {}
        Level * const mLevel;
};

#endif // EDITACTION_H

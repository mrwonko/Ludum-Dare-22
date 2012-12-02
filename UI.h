#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class UIElement;

class UI : public sf::Drawable
{
    public:
        UI();
        virtual ~UI();

        /** Element will henceforth be managed by the UI, i.e. will be deleted when the UI is deleted. Get it via name. Any previous item of the same name will be deleted. **/
        void AddElement(const std::string& name, UIElement* element);

        /** Returns NULL if no such UIElement exists. **/
        UIElement* GetElement(const std::string& name);

    private:
		virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;

        typedef std::map<std::string, UIElement*> ElementMap;
        ElementMap mElements;
};

#endif // UI_H

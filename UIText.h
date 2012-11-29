#ifndef UITEXT_H
#define UITEXT_H

#include "UIElement.h"

#include <SFML/Graphics/Text.hpp>

class UIText : public UIElement
{
    public:
        UIText();
        virtual ~UIText();

        void SetText(const std::string& text)
        {
            mText.setString(text);
        }

        void SetCharacterSize(unsigned int size)
        {
            mText.setCharacterSize(size);
        }

    private:
        sf::Text mText;

        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
};

#endif // UITEXT_H

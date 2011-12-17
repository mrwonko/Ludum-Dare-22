#ifndef STATICRECT_H
#define STATICRECT_H

#include "Object.h"

class StaticRect : public Object
{
    public:
        StaticRect(Level* const level);
        virtual ~StaticRect();

        virtual const std::string GetType() const { return "StaticRect"; }

        virtual const bool Serialize(std::ostream& out_stream) const;
        virtual const bool Deserialize(std::istream& stream);
    protected:
        virtual void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;
};

#endif // STATICRECT_H

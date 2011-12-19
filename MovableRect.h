#ifndef MOVABLERECT_H
#define MOVABLERECT_H

#include "StaticRect.h"


class MovableRect : public StaticRect
{
    public:
        MovableRect(Level * const level);
        virtual ~MovableRect();

        virtual void Update(unsigned int deltaT_msec);
        virtual const std::string GetType() const { return "MovableRect"; }
    protected:
        virtual void CreateBody(const b2Vec2& pos);
        virtual void CreateFixture(const float hsizeX, const float hsizeY);
};

#endif // MOVABLERECT_H

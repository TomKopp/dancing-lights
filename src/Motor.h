#pragma once

namespace Zalari
{
    class Motor
    {
    public:
        virtual void setPosition(double angleRad) = 0;
        virtual void disable() = 0;
        virtual void enable() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
    };
}

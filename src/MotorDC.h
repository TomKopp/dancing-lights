#pragma once

#include "Motor.h"

namespace Zalari
{
    class MotorDC : public Motor
    {
    private:
        const uint8_t p1;
        const uint8_t p2;
        signed char direction;
        bool disabled;
        unsigned long nextTick;
        unsigned long runtime;
        unsigned long endTime;

        enum class State : char
        {
            STOP,
            POS,
            NEG
        };

        State state;
        State stateOld;

    public:
        MotorDC(uint8_t, uint8_t);
        ~MotorDC();

        void setPosition(double) override;
        void disable() override;
        void enable() override;
        void update() override;
        void render() override;
    };

} // namespace Zalari

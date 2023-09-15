#pragma once

#include "Motor.h"

namespace Zalari
{
    class MotorDC : public Motor
    {
    private:
        const uint8_t p1;
        const uint8_t p2;
        int direction;
        bool disabled;
        uint64_t nextTick;
        uint64_t runtime;
        uint64_t endTime;

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

    private:
        void setState();
    };

} // namespace Zalari

#pragma once

#include "Motor.h"

namespace Zalari
{
    class MotorSynchron : public Motor
    {
    private:
        const uint8_t p1;
        const uint8_t p2;
        double direction;
        bool disabled;
        bool dirty;
        uint64_t nextTick;

    public:
        MotorSynchron(uint8_t, uint8_t);
        ~MotorSynchron();

        void setPosition(double) override;
        void disable() override;
        void enable() override;
        void update() override;
        void render() override;
    };

} // namespace Zalari

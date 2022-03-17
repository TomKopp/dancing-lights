#pragma once

#include "Motor.h"

namespace Zalari
{
    /**
     * @see @link https://cookierobotics.com/042/
     */
    class MotorStepper28BYJ_48 : public Motor
    {
    private:
        const uint8_t pA1; // pink
        const uint8_t pA2; // orange
        const uint8_t pB1; // yellow
        const uint8_t pB2; // blue
        uint8_t seqPhase;
        int32_t steps;
        bool disabled;
        bool dirty;

        bool seq[8][4] = {
            // Half-Step
            // {1, 0, 0, 0},
            // {1, 0, 1, 0},
            // {0, 0, 1, 0},
            // {0, 1, 1, 0},

            // {0, 1, 0, 0},
            // {0, 1, 0, 1},
            // {0, 0, 0, 1},
            // {1, 0, 0, 1},

            // Wave Drive 1 coil
            // {0, 1, 0, 0},
            // {0, 0, 0, 1},
            // {1, 0, 0, 0},
            // {0, 0, 1, 0},

            // {0, 1, 0, 0},
            // {0, 0, 0, 1},
            // {1, 0, 0, 0},
            // {0, 0, 1, 0},

            // Full-Step 2 coils
            {1, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 1},
            {1, 0, 0, 1},

            {1, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 1},
            {1, 0, 0, 1},
        };

    public:
        MotorStepper28BYJ_48(uint8_t, uint8_t, uint8_t, uint8_t);
        ~MotorStepper28BYJ_48();

        void setPosition(double) override;
        void setPosition(int32_t);
        void disable() override;
        void enable() override;
        void update() override;
        void render() override;
    };
}

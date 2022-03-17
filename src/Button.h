#pragma once

#include <Arduino.h>

namespace Zalari
{
    /**
     * @see @link https://github.com/e-tinkers/button
     */
    class Button
    {
    private:
        const uint8_t p;
        uint8_t limbo;
        uint8_t state;
        uint8_t lastState;

    public:
        explicit Button(uint8_t pin) : p(pin), limbo(0), state(1), lastState(0)
        {
            /**
             * If pin is configured with INPUT_PULLUP
             * this causes the pin to read HIGH when the switch is open,
             * and LOW when the switch is pressed
             */
            pinMode(p, INPUT_PULLUP);
        }

        void sample()
        {
            // digitalRead returns LOW (0x0) if active or HIGH (0x1) if inactive
            limbo = (limbo << 1) | digitalRead(p);

            // is settled
            if (limbo == 0xff || limbo == 0x00)
            {
                lastState = state;
                state = limbo;
            }
        }

        bool isActive()
        {
            return (state == 0x00);
        }

        bool isOpen()
        {
            return (state == 0xff);
        }

        bool isKeyDown()
        {
            return (state == 0x00 && lastState == 0xff);
        }
        bool isKeyUp()
        {
            return (state == 0xff && lastState == 0x00);
        }
    };
}

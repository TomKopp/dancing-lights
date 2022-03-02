#include <Arduino.h>
#include <cmath>
#include "MotorStepper28BYJ_48.h"
#include "Config.h"

namespace Zalari
{
    MotorStepper28BYJ_48::MotorStepper28BYJ_48(uint8_t pinCoilA1, uint8_t pinCoilA2, uint8_t pinCoilB1, uint8_t pinCoilB2)
        : pA1(pinCoilA1), pA2(pinCoilA2), pB1(pinCoilB1), pB2(pinCoilB2), seqPhase(0), steps(0), disabled(true)
    {
        pinMode(pA1, OUTPUT);
        pinMode(pA2, OUTPUT);
        pinMode(pB1, OUTPUT);
        pinMode(pB2, OUTPUT);
        digitalWrite(pA1, LOW);
        digitalWrite(pA2, LOW);
        digitalWrite(pB1, LOW);
        digitalWrite(pB2, LOW);
    }

    MotorStepper28BYJ_48::~MotorStepper28BYJ_48()
    {
        digitalWrite(pA1, LOW);
        digitalWrite(pA2, LOW);
        digitalWrite(pB1, LOW);
        digitalWrite(pB2, LOW);
    }

    void MotorStepper28BYJ_48::setPosition(double angleRad)
    {
        /**
         * @see @link https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/
         * There are 32 steps per revolution (360°/11.25° = 32)
         * In addition, the motor has a 1/64 reduction gear set. (Actually its 1/63.68395)
         * 32*63.68395 steps per revolution = 2037.8864 ~ 2038 steps
         */
        steps = (int) (1019 * angleRad / PI);
        disabled = false;
        dirty = true;
    }

    void MotorStepper28BYJ_48::setPosition(int32_t steps)
    {
        this->steps = steps;
        disabled = false;
        dirty = true;
    }

    void MotorStepper28BYJ_48::disable()
    {
        disabled = true;
        dirty = true;
    }

    void MotorStepper28BYJ_48::enable()
    {
        disabled = false;
        dirty = true;
    }

    void MotorStepper28BYJ_48::update()
    {
        if (steps == 0 || disabled)
            return;

        int sign = (steps > 0) - (steps < 0);
        // seqPhase = (seqPhase + sign) % 8;
        // @see @link https://en.wikipedia.org/wiki/Modulo_operation
        // Quotient and remainder using Euclidean division
        seqPhase = (seqPhase + sign) - 8 * floor(seqPhase / 8);
        steps -= sign;
        dirty = true;
    }

    void MotorStepper28BYJ_48::render()
    {
        if (!dirty)
            return;
        if (disabled)
        {
            digitalWrite(pA1, LOW);
            digitalWrite(pA2, LOW);
            digitalWrite(pB1, LOW);
            digitalWrite(pB2, LOW);
            return;
        }
        digitalWrite(pA1, seq[seqPhase][0]);
        digitalWrite(pA2, seq[seqPhase][1]);
        digitalWrite(pB1, seq[seqPhase][2]);
        digitalWrite(pB2, seq[seqPhase][3]);
    }
}

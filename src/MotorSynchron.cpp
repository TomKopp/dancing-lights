#include <Arduino.h>
#include <cmath>
#include "MotorSynchron.h"
#include "Config.h"

namespace Zalari
{
    MotorSynchron::MotorSynchron(uint8_t pin1, uint8_t pin2) : p1(pin1), p2(pin2)
    {
        pinMode(p1, OUTPUT);
        pinMode(p2, OUTPUT);
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
        nextTick = millis();
        disabled = true;
        direction = 0.0;
        state = MotorSynchron::State::STOP;
    }

    MotorSynchron::~MotorSynchron()
    {
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
    }

    void MotorSynchron::setPosition(double angleRad)
    {
        // calc time to turn from angle
        direction = angleRad;
    };

    void MotorSynchron::disable()
    {
        disabled = true;
    };

    void MotorSynchron::enable()
    {
        disabled = false;
    };

    void MotorSynchron::update()
    {
        stateOld = state;

        if (direction > 0)
            state = MotorSynchron::State::POS;
        else
            state = MotorSynchron::State::NEG;
    };

    void MotorSynchron::render()
    {
        // Emergency stop
        if (disabled)
        {
            digitalWrite(p1, LOW);
            digitalWrite(p2, LOW);
            return;
        }

        // If nothing changed, do nothing
        if (state == stateOld)
            return;

        // if state changed
        // STOP the motor for x millis
        if (!dirty)
        {
            digitalWrite(p1, LOW);
            digitalWrite(p2, LOW);
            dirty = true;
            nextTick = millis() + DEBOUNCEDELAY;
        }

        // if motor stopped for x millis, set new state
        if (dirty && millis() > nextTick)
        {
            // Change the state
            switch (state)
            {
            case MotorSynchron::State::POS:
                digitalWrite(p1, HIGH);
                digitalWrite(p2, LOW);
                break;
            case MotorSynchron::State::NEG:
                digitalWrite(p1, LOW);
                digitalWrite(p2, HIGH);
                break;
            default: // State::Stopped:
                digitalWrite(p1, LOW);
                digitalWrite(p2, LOW);
                break;
            }

            dirty = false;
        }
    }
};

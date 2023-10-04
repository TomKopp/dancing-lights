#include <Arduino.h>
#include <cmath>
#include "MotorSynchron.h"
#include "Config.h"

namespace Zalari
{
    MotorSynchron::MotorSynchron(uint8_t pin1, uint8_t pin2)
        : p1(pin1), p2(pin2), direction(0.0), state(MotorSynchron::State::STOP), stateOld(MotorSynchron::State::STOP), disabled(true), dirty(false)
    {
        pinMode(p1, OUTPUT);
        pinMode(p2, OUTPUT);
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
        nextTick = millis();
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
    }

    void MotorSynchron::disable()
    {
        disabled = true;
    }

    void MotorSynchron::enable()
    {
        disabled = false;
    }

    void MotorSynchron::setState()
    {
        if (stateOld == MotorSynchron::State::NEG && state == MotorSynchron::State::POS ||
            stateOld == MotorSynchron::State::POS && state == MotorSynchron::State::NEG)
            state = MotorSynchron::State::STOP;

        return;
    }

    void MotorSynchron::update()
    {
        switch ((direction > 0) - (direction < 0))
        {
        case 1:
            state = MotorSynchron::State::POS;
            // allowed() ? set state : set to stop
            setState();
            break;
        case -1:
            state = MotorSynchron::State::NEG;
            // allowed() ? set state : set to stop
            setState();
            break;
        default:
            state = MotorSynchron::State::STOP;
            break;
        }
    }

    void MotorSynchron::render()
    {
        // Emergency stop
        if (disabled)
        {
            digitalWrite(p1, LOW);
            digitalWrite(p2, LOW);
            stateOld = MotorSynchron::State::STOP;
            return;
        }

        // If nothing changed, do nothing
        if (stateOld == state)
            return;

        // if state changed
        // STOP the motor for x millis

        // if motor stopped for x millis, set new state
        if (millis() > nextTick)
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
                nextTick = millis() + 200;
                break;
            }
            stateOld = state;
        }
    }
};

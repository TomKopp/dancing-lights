#include <Arduino.h>
#include <cmath>
#include "MotorDC.h"
#include "Config.h"

namespace Zalari
{
    MotorDC::MotorDC(uint8_t pin1, uint8_t pin2)
        : p1(pin1), p2(pin2), direction(0.0), state(MotorDC::State::STOP), disabled(true), dirty(false)
    {
        pinMode(p1, OUTPUT);
        pinMode(p2, OUTPUT);
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
        nextTick = millis();
    }

    MotorDC::~MotorDC()
    {
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
    }

    void MotorDC::setPosition(double angleRad)
    {
        // calc time to turn from angle
        direction = angleRad;
    }

    void MotorDC::disable()
    {
        disabled = true;
    }

    void MotorDC::enable()
    {
        disabled = false;
    }

    void MotorDC::setState()
    {
        if (stateOld == MotorDC::State::NEG && state == MotorDC::State::POS ||
            stateOld == MotorDC::State::POS && state == MotorDC::State::NEG)
            state = MotorDC::State::STOP;

        return;
    }

    void MotorDC::update()
    {
        switch ((direction > 0) - (direction < 0))
        {
        case 1:
            state = MotorDC::State::POS;
            // allowed() ? set state : set to stop
            setState();
            break;
        case -1:
            state = MotorDC::State::NEG;
            // allowed() ? set state : set to stop
            setState();
            break;
        default:
            state = MotorDC::State::STOP;
            break;
        }
    }

    void MotorDC::render()
    {
        // Emergency stop
        if (disabled)
        {
            analogWrite(p1, 0);
            analogWrite(p2, 0);
            stateOld = MotorDC::State::STOP;
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
            case MotorDC::State::POS:
                analogWrite(p1, 255);
                analogWrite(p2, 0);
                break;
            case MotorDC::State::NEG:
                analogWrite(p1, 0);
                analogWrite(p2, 255);
                break;
            default: // State::Stopped:
                analogWrite(p1, 0);
                analogWrite(p2, 0);
                nextTick = millis() + 20;
                break;
            }
            stateOld = state;
        }
    }
};

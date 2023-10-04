#include <Arduino.h>
#include <cmath>
#include "MotorDC.h"
#include "Config.h"

namespace Zalari
{
    /*
     ~17000rpm @ 1:48 ratio => 354rpm axle ~ 5.8rps
        => ~2000˚/s => ~35rad/s
        => ~0.35rad/tick == 0.35rad/10ms
    */
    MotorDC::MotorDC(uint8_t pin1, uint8_t pin2)
        : p1(pin1), p2(pin2), direction(0), state(MotorDC::State::STOP), stateOld(MotorDC::State::STOP), runtime(0), disabled(true)
    {
        pinMode(p1, OUTPUT);
        pinMode(p2, OUTPUT);
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
        nextTick = endTime = millis();
    }

    MotorDC::~MotorDC()
    {
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
    }

    void MotorDC::setPosition(double angleRad)
    {
        // calc time to turn from angle
        runtime = (std::abs(angleRad) / 35) * 1000;
        direction = (angleRad > 0) - (angleRad < 0);
    }

    void MotorDC::disable()
    {
        disabled = true;
    }

    void MotorDC::enable()
    {
        disabled = false;
        endTime = millis() + runtime;
        Serial.print("Run ");
        Serial.println(runtime);
    }

    void MotorDC::update()
    {
        if (millis() > endTime)
        {
            // state = MotorDC::State::STOP;
            direction = 0;
        }

        switch (direction)
        {
        case 1:
            if (stateOld == MotorDC::State::NEG)
                state = MotorDC::State::STOP;
            else
                state = MotorDC::State::POS;
            break;
        case -1:
            if (stateOld == MotorDC::State::POS)
                state = MotorDC::State::STOP;
            else
                state = MotorDC::State::NEG;
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
            break;
        }
        stateOld = state;
    }
};

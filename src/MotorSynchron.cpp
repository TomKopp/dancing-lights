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
        ;
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
    };

    void MotorSynchron::disable()
    {
        disabled = true;
        dirty = true;
    };

    void MotorSynchron::enable()
    {
        disabled = false;
    };

    void MotorSynchron::update()
    {
        if (millis() > nextTick)
        {
            dirty = true;
            nextTick += DEBOUNCEDELAY;
        }
        else
        {
            dirty = false;
        }
    };

    void MotorSynchron::render()
    {
        if (!dirty)
            return;
        if (disabled)
        {
            digitalWrite(p1, LOW);
            digitalWrite(p2, LOW);
            return;
        }

        if (direction > 0)
        {
            digitalWrite(p1, HIGH);
            digitalWrite(p2, LOW);
        }
        else
        {
            digitalWrite(p1, LOW);
            digitalWrite(p2, HIGH);
        }
    };

}

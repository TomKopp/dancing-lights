#include <Arduino.h>
#include "Config.h"
#include "Button.h"
#include "MotorStepper28BYJ_48.h"

using namespace Zalari;

Button btn1(15);
Button btn2(4);
Button btn3(18);
Button btn4(19);
MotorStepper28BYJ_48 m1(12, 27, 14, 13); // pink orange yellow blue
MotorStepper28BYJ_48 m2(25, 32, 33, 26); // pink orange yellow blue
uint8_t ledState = LOW;
uint8_t skippedFrames = 0;
uint64_t nextTick;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    nextTick = millis();
}

void loop()
{
    skippedFrames = 0;
    while (millis() > nextTick && skippedFrames < MAX_SKIPPEDFRAMES)
    {
        //* Update
        btn1.sample();
        btn2.sample();
        btn3.sample();
        btn4.sample();
        m1.update();
        m2.update();

        ledState = LOW;

        //* Actions
        if (btn1.isActive() || btn2.isActive() || btn3.isActive() || btn4.isActive())
            ledState = HIGH;

        if (btn1.isKeyDown() && !btn2.isActive())
            m1.setPosition(2 * PI);
        if (btn2.isKeyDown() && !btn1.isActive())
            m1.setPosition(-2 * PI);
        if (btn1.isActive() && btn2.isKeyDown())
            m1.disable();

        if (btn3.isKeyDown() && !btn4.isActive())
            m2.setPosition(2 * PI);
        if (btn4.isKeyDown() && !btn3.isActive())
            m2.setPosition(-2 * PI);
        if (btn3.isActive() && btn4.isKeyDown())
            m2.disable();

        nextTick += DEBOUNCEDELAY;
        skippedFrames++;
    }

    //* Render
    digitalWrite(LED_BUILTIN, ledState);

    m1.render();
    m2.render();
}

#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "Button.h"
#include "MotorStepper28BYJ_48.h"
#include "MotorSynchron.h"
#include "MotorDC.h"

using namespace Zalari;

Button btn1(15);
Button btn2(4);
// Button btn3(18);
// Button btn4(19);
// MotorStepper28BYJ_48 m1(12, 27, 14, 13); // pink orange yellow blue
// MotorStepper28BYJ_48 m2(25, 32, 33, 26); // pink orange yellow blue
uint8_t ledState = LOW;
uint8_t skippedFrames = 0;
uint64_t nextTick;

// MotorSynchron m1(12, 13);
// MotorSynchron m2(25, 26);
MotorDC m1(12, 13);

void setup()
{   
    char ssid[] = "ZALARI Gast";
    char pass[] = "galarizast2019!";
    Serial.begin(115200);
    while (!Serial)
        ;
       

    while (Serial.available() <= 0)
    {
        // send a capital A
        Serial.print('A');
        delay(500);
    }
    Serial.println("\nYou're connected.");

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println(WiFi.status());

        Serial.println("Try to connect");
    }
    Serial.println("Connect to internet");
    pinMode(LED_BUILTIN, OUTPUT);

    nextTick = millis();
}

void loop()
{
    skippedFrames = 0;
    while (millis() > nextTick && skippedFrames < MAX_SKIPPEDFRAMES)
    {
        ledState = LOW;

        //* Actions
        // if (btn1.isActive() || btn2.isActive() || btn3.isActive() || btn4.isActive())
        if (btn1.isActive() || btn2.isActive())
            ledState = HIGH;

        // if (btn1.isKeyDown() && !btn2.isActive())
        //     m1.setPosition(2 * PI);
        // if (btn2.isKeyDown() && !btn1.isActive())
        //     m1.setPosition(-2 * PI);
        // if (btn1.isActive() && btn2.isKeyDown())
        //     m1.disable();

        // if (btn3.isKeyDown() && !btn4.isActive())
        //     m2.setPosition(2 * PI);
        // if (btn4.isKeyDown() && !btn3.isActive())
        //     m2.setPosition(-2 * PI);
        // if (btn3.isActive() && btn4.isKeyDown())
        //     m2.disable();

        if (btn1.isActive())
        {
            m1.setPosition(1);
            m1.enable();
        }
        if (btn2.isActive())
        {
            m1.setPosition(-1);
            m1.enable();
        }
        if (btn1.isOpen() && btn2.isOpen())
            m1.disable();

        // if (btn3.isActive())
        // {
        //     m2.setPosition(1);
        //     m2.enable();
        // }
        // if (btn4.isActive())
        // {
        //     m2.setPosition(-1);
        //     m2.enable();
        // }
        // if (btn3.isOpen() && btn4.isOpen())
        //     m2.disable();

        //* Update
        btn1.update();
        btn2.update();
        // btn3.update();
        // btn4.update();
        m1.update();
        // m2.update();

        nextTick += DEBOUNCEDELAY;
        skippedFrames++;
    }

    //* Render
    digitalWrite(LED_BUILTIN, ledState);

    m1.render();
    // m2.render();
}

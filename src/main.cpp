#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include <array>

#include "Config.h"
#include "Button.h"
#include "MotorDC.h"
#include "MessageHandler.h"

using namespace Zalari;

Button btn1(15);
Button btn2(4);
Button btn3(18);
Button btn4(19);

MotorDC m1(12, 14);
MotorDC m2(26, 27);

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
MessageHandler msgHandler(std::array<Motor *, 2>{&m1, &m2});

uint8_t ledState = LOW;
uint8_t skippedFrames = 0;
unsigned long nextTick;

void setup()
{
    // Connect Serial
    Serial.begin(BAUD_RATE);
    // while (!Serial)
    //     ;

    // while (Serial.available() <= 0)
    // {
    //     // send a capital A
    //     Serial.print('A');
    //     delay(500);
    // }
    // Serial.println("\nYou're connected.");

    // Connect WiFi
    Serial.println("Try to connect WiFi");
    Serial.println(SSID);
    Serial.println(PASS);
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Connected to WiFi");

    // Connect MQTT
    if (!mqttClient.connect(MQTT_HOST))
    {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
        while (1)
            ;
    }
    Serial.println("You're connected to the MQTT broker!");

    // Set message handler
    while (Serial.available())
        Serial.read();
    Serial.onReceive(std::bind(&MessageHandler::handleSerialMessage, msgHandler, &Serial), true);
    mqttClient.onMessage(std::bind(&MessageHandler::handleMqttMessage, msgHandler, std::placeholders::_1, std::placeholders::_2));
    mqttClient.subscribe(MQTT_TOPIC);

    // Generic setup
    // pinMode(LED_BUILTIN, OUTPUT);
    nextTick = millis();
}

void loop()
{
    //* Preconditions
    // call poll() regularly to allow the library to receive MQTT messages and
    // send MQTT keep alive which avoids being disconnected by the broker
    mqttClient.poll();
    // if (Serial.available()) {
    //     msgHandler.handleSerialMessage(&Serial);
    // }

    //* Time sensitive calculations
    skippedFrames = 0;
    while (millis() > nextTick && skippedFrames < MAX_SKIPPEDFRAMES)
    {
        //* PreAction
        ledState = LOW;
        btn1.update();
        btn2.update();
        btn3.update();
        btn4.update();

        //* Actions
        // if (btn1.isActive() || btn2.isActive() || btn3.isActive() || btn4.isActive())
        //     ledState = HIGH;

        // if (btn1.isActive())
        // {
        //     m1.setPosition(1);
        //     m1.enable();
        // }
        // if (btn2.isActive())
        // {
        //     m1.setPosition(-1);
        //     m1.enable();
        // }
        // if (btn1.isOpen() && btn2.isOpen())
        //     m1.disable();

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

        //* PostAction
        m1.update();
        m2.update();

        nextTick += DEBOUNCEDELAY;
        skippedFrames++;
    }

    //* Render
    // digitalWrite(LED_BUILTIN, ledState);

    m1.render();
    m2.render();
}

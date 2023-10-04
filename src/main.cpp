#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include <array>

#include "Config.h"
#include "Button.h"
#include "MotorDC.h"
#include "MessageHandler.h"

using namespace Zalari;

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
#ifdef LED_BUILTIN
    pinMode(LED_BUILTIN, OUTPUT);
#endif

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
    Serial.print("Try to connect WiFi: ");
    Serial.println(SSID);
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
#ifdef LED_BUILTIN
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
#endif
        delay(1000);
    }
    Serial.println("Connected to WiFi");

    // Connect MQTT
    if (!mqttClient.connect(MQTT_HOST))
    {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
#ifdef LED_BUILTIN
        digitalWrite(LED_BUILTIN, HIGH);
#endif
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
#ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, LOW);
#endif
    nextTick = millis();
}

void loop()
{
    //* Preconditions
    // call poll() regularly to allow the library to receive MQTT messages and
    // send MQTT keep alive which avoids being disconnected by the broker
    mqttClient.poll();

    //* Time sensitive calculations
    skippedFrames = 0;
    while (millis() > nextTick && skippedFrames < MAX_SKIPPEDFRAMES)
    {
        //* PreAction
        ledState = LOW;

        //* Actions


        //* PostAction
        m1.update();
        m2.update();

        nextTick += DEBOUNCEDELAY;
        skippedFrames++;
    }

    //* Render
    m1.render();
    m2.render();
}

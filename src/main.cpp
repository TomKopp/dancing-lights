#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>

#include "Config.h"
#include "Button.h"
#include "MotorDC.h"

using namespace Zalari;

const char *broker = "broker.emqx.io";
uint16_t port = 1883;
const char *topic = "/nodejs/dancingLights";
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

Button btn1(15);
Button btn2(4);
Button btn3(18);
Button btn4(19);
uint8_t ledState = LOW;
uint8_t skippedFrames = 0;
unsigned long nextTick;

MotorDC m1(12, 13);
MotorDC m2(14, 27);

void onMqttMessage(int messageSize)
{
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', duplicate = ");
    Serial.print(mqttClient.messageDup() ? "true" : "false");
    Serial.print(", QoS = ");
    Serial.print(mqttClient.messageQoS());
    Serial.print(", retained = ");
    Serial.print(mqttClient.messageRetain() ? "true" : "false");
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Stream interface to print the contents
    while (mqttClient.available())
    {
        Serial.print((char)mqttClient.read());
    }
    Serial.println();

    Serial.println();
}

void setup()
{
    Serial.begin(BAUD_RATE);
    while (!Serial)
        ;

    while (Serial.available() <= 0)
    {
        // send a capital A
        Serial.print('A');
        delay(500);
    }

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

    if (!mqttClient.connect(broker, port))
    {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());

        while (1)
            ;
    }
    Serial.println("You're connected to the MQTT broker!");
    mqttClient.onMessage(onMqttMessage);
    int subscribeQos = 1;
    mqttClient.subscribe(topic, subscribeQos);

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
        if (btn1.isActive() || btn2.isActive() || btn3.isActive() || btn4.isActive())
            ledState = HIGH;

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

        if (btn3.isActive())
        {
            m2.setPosition(1);
            m2.enable();
        }
        if (btn4.isActive())
        {
            m2.setPosition(-1);
            m2.enable();
        }
        if (btn3.isOpen() && btn4.isOpen())
            m2.disable();

        //* Update
        btn1.update();
        btn2.update();
        btn3.update();
        btn4.update();
        m1.update();
        m2.update();

        nextTick += DEBOUNCEDELAY;
        skippedFrames++;
    }

    //* Render
    digitalWrite(LED_BUILTIN, ledState);

    m1.render();
    m2.render();

    // call poll() regularly to allow the library to receive MQTT messages and
    // send MQTT keep alives which avoids being disconnected by the broker
    mqttClient.poll();
}

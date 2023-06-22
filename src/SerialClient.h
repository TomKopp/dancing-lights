#pragma once

#include <Arduino.h>
#include <ArduinoMqttClient.h>

namespace Zalari
{
    class SerialClient
    {
        typedef std::function<void(int messageSize)> MessageCallback;

    private:
        static const unsigned int FORCE_CHAR_BUFFER_SIZE = 3; // In byte
        unsigned long baudRate;
        String forceCharBuffer;
        bool isSettingsReady;
        unsigned int motorId;
        MessageCallback _onMessage;

    public:
        explicit SerialClient(unsigned long baud = 115200) : baudRate(baud), forceCharBuffer(""), isSettingsReady(false), motorId(0), _onMessage(NULL)
        {
            forceCharBuffer.reserve(FORCE_CHAR_BUFFER_SIZE);
        };

        /**
         * Establish Connection
         */
        void establishContact()
        {
            Serial.begin(baudRate);
            while (!Serial)
                ;

            while (Serial.available() <= 0)
            {
                // send a capital A
                Serial.print('A');
                delay(500);
            }
            Serial.println("\nYou're connected.");
        }

        void onMessage(MessageCallback callback)
        {
            _onMessage = callback;
        }

        void handleMqttMessage(MqttClient *client, int messageSize)
        {
            // we received a message, print out the topic and contents
            Serial.print("Received a message with topic '");
            Serial.print(client->messageTopic());
            Serial.print("', duplicate = ");
            Serial.print(client->messageDup() ? "true" : "false");
            Serial.print(", QoS = ");
            Serial.print(client->messageQoS());
            Serial.print(", retained = ");
            Serial.print(client->messageRetain() ? "true" : "false");
            Serial.print("', length ");
            Serial.print(messageSize);
            Serial.println(" bytes:");

            // use the Stream interface to print the contents
            while (client->available())
            {
                Serial.print((char)client->read());
            }
            Serial.println();
            Serial.println();
        }

        /**
         * SerialEvent
         *
         * read Serial
         * 0=20,0,100&1=0,0,30&7=200,0,255\r\n
         */
        void poll()
        {
            // if (settings == nullptr)
            // {
            //     settings = new TactileSettings();
            // }

            while (Serial.available())
            {
                char inChar = (char)Serial.read();

                switch (inChar)
                {
                case '=':
                    // buffer has motorId -> set _motorId
                    motorId = forceCharBuffer.toInt();
                    forceCharBuffer = "";
                    break;
                case '\r':
                case '\n':
                    // send TactileSettings
                    isSettingsReady = true;

                    // clear rest buffer
                    if (Serial.peek() == '\n')
                    {
                        Serial.read();
                    }
                    // explicit fall through
                case ',':
                case '&':
                    // buffer has next force as string
                    // settings->appendForce(motorId, forceCharBuffer.toInt());
                    forceCharBuffer = "";
                    break;
                default:
                    forceCharBuffer += inChar;
                }
            }

            if (isSettingsReady && _onMessage)
            {
                _onMessage(1337);
                isSettingsReady = false;
            }
        }
    };
}

#pragma once

#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <array>
#include <string>

#include "Motor.h"

namespace Zalari
{
    class MessageHandler
    {
    private:
        std::array<Motor *, 2> motors;

        /**
         * @example
         * 0=22.89&1=3\r\n
         */
        void readFromBuffer(Stream *stream)
        {
            std::string buffer;
            bool isReady = false;
            std::size_t motorId = 0;

            while (stream->available())
            {
                char inChar = (char)stream->read();
                switch (inChar)
                {
                case '=':
                    // buffer has motorId -> set _motorId
                    try
                    {
                        motorId = std::stoi(buffer);
                        Serial.print("mId ");
                        Serial.println(motorId);
                    }
                    catch(const std::exception& e)
                    {
                        Serial.println("ENOMID");
                    }
                    buffer.clear();
                    break;
                case '\r':
                case '\n':
                    // clear dangling new-line
                    while (stream->peek() == '\n')
                        stream->read();
                    // send TactileSettings
                    isReady = true;
                    // explicit fall through
                case '&':
                    // buffer has next position as string
                    try
                    {
                        motors.at(motorId)->setPosition(std::stod(buffer));
                        Serial.print("Pos ");
                        Serial.println(std::stod(buffer));
                    }
                    catch(const std::exception& e)
                    {
                        Serial.println("ENOPOS");
                    }
                    buffer.clear();
                    break;
                default:
                    buffer += inChar;
                }
            }

            if (isReady)
            {
                // Set motors into motion
                // ToDo don't enable motors that were not send
                for (const auto &m : motors)
                    m->enable();
            }
        }

    public:
        explicit MessageHandler(std::array<Motor *, 2> motors) : motors(motors)
        {
        }

        void handleMqttMessage(MqttClient *client, int messageSize)
        {
            // we received a message, print out the topic and contents
            // Serial.print("Received a message with topic '");
            // Serial.print(client->messageTopic());
            // Serial.print("', duplicate = ");
            // Serial.print(client->messageDup() ? "true" : "false");
            // Serial.print(", QoS = ");
            // Serial.print(client->messageQoS());
            // Serial.print(", retained = ");
            // Serial.print(client->messageRetain() ? "true" : "false");
            // Serial.print("', length ");
            // Serial.print(messageSize);
            // Serial.println(" bytes");

            readFromBuffer(client);
        }

        void handleSerialMessage(HardwareSerial *serial)
        {
            readFromBuffer(serial);
        }
    };
}

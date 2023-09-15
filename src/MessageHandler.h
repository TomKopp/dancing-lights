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
                    motorId = std::stoi(buffer);
                    // Serial.print("motorId ");
                    // Serial.println(motorId);
                    buffer.clear();
                    break;
                case '\r':
                case '\n':
                    // send TactileSettings
                    isReady = true;
                    // clear dangling new-line
                    if (stream->peek() == '\n')
                        stream->read();
                    // explicit fall through
                case '&':
                    // buffer has next force as string
                    // Serial.print("Pos ");
                    // Serial.println(std::stod(buffer));
                    motors.at(motorId)->setPosition(std::stod(buffer));
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
                isReady = false;
            }
        }

    public:
        MessageHandler(std::array<Motor *, 2> motors) : motors(motors)
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
            // Serial.println(" bytes:");

            readFromBuffer(client);
        }

        void handleSerialMessage(HardwareSerial *serial)
        {
            readFromBuffer(serial);
        }
    };
}

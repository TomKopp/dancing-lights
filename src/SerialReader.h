#pragma once

#include <Arduino.h>

namespace Zalari
{
    class SerialReader
    {
    private:
        // TactileSettings *settings = nullptr;
        // bool isSettingsReady = false;
        int FORCE_CHAR_BUFFER_SIZE = 3; // In byte
        int motorId;
        String forceCharBuffer = "";
        // {
        // forceCharBuffer.reserve(FORCE_CHAR_BUFFER_SIZE);
        // }

    public:
        /**
         * Establish Connection
         */
        void establishContact()
        {
            Serial.begin(57600);
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

        /**
         * SerialEvent
         *
         * read Serial
         * 0=20,0,100&1=0,0,30&7=200,0,255\r\n
         */
        void mySerialEvent()
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
                    // isSettingsReady = true;

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
        }
    };
}

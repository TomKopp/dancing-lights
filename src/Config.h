#pragma once

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

namespace Zalari
{
    static const uint8_t TICKS_PER_SECOND = 100; // in Hertz
    static const uint8_t MAX_SKIPPEDFRAMES = 10;
    static const unsigned long DEBOUNCEDELAY = 1000 / TICKS_PER_SECOND; // In milliseconds

    static const unsigned long BAUD_RATE = 115200;

    static const char *SSID = "ZALARI Gast";
    static const char *PASS = "galarizast2019!";

    static const char *MQTT_HOST = "broker.emqx.io";
    // static const uint16_t MQTT_PORT = 1883; // unnecessary
    static const char *MQTT_TOPIC = "/nodejs/dancingLights";
}

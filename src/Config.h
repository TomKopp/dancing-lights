#pragma once

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

namespace Zalari
{
    static const uint16_t TICKS_PER_SECOND = 100; // in Hertz
    static const uint8_t MAX_SKIPPEDFRAMES = 10;


    static const uint16_t DEBOUNCEDELAY = 1000 / TICKS_PER_SECOND; // In milliseconds
}

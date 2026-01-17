#include <stdint.h>
#include "arduino_uno.h"
#include "atmega328p.h"
#include "digital.h"
#include "timing.h"

// This example shows the usage of delayMicroseconds.
// This function was developed for really small delays
// in mind. To do a 1 second delay we call delayMicroseconds
// 1000 times sending 1000 microseconds each time.
// Here the built-in led in the Arduino UNO R3 will blink
// for 1 second and the turn off for 1 second.

// This is just an approximation
void auxDelay1Second()
{
    uint16_t ms = 1000;
    while (ms--)
        delayMicroseconds(1000);
}

int main()
{
    pinMode(LED_BUILTIN, OUTPUT);

    while (1)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        auxDelay1Second();

        digitalWrite(LED_BUILTIN, LOW);
        auxDelay1Second();
    }

    return 0;
}
#include <stdint.h>
#include "arduino_uno.h"
#include "atmega328p.h"
#include "digital.h"

// This example uses the Led built into the Arduino UNO
// and blinks it using the digitalWrite function.
// Since at the time of the creation of this example
// there's no implementation of the delay function
// we use long for loops to waste some time.

int main()
{
    pinMode(LED_BUILTIN, OUTPUT);

    while (1)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        for (long i = 0; i < 1000000; i++)
        {
            __asm__("nop");
        }

        digitalWrite(LED_BUILTIN, LOW);
        for (long i = 0; i < 1000000; i++)
        {
            __asm__("nop");
        }
    }

    return 0;
}
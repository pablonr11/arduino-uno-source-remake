#include <stdint.h>
#include "arduino_uno.h"
#include "atmega328p.h"
#include "digital.h"

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
#include <stdint.h>
#include "arduino_uno.h"
#include "atmega328p.h"
#include "digital.h"

int main()
{
    pinMode(LED_BUILTIN, OUTPUT);

    while (1)
    {
        PORTB |= (1 << 5);
        for (long i = 0; i < 100000; i++)
        {
            __asm__("nop");
        }

        PORTB &= ~(1 << 5);
        for (long i = 0; i < 100000; i++)
        {
            __asm__("nop");
        }
    }

    return 0;
}
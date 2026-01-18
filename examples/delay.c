#include <stdint.h>
#include "arduino_uno.h"
#include "atmega328p.h"
#include "digital.h"
#include "timing.h"

// This example uses the delay function to blink
// the built-in led in the Arduino UNO every second

int main()
{
    initTiming();

    pinMode(LED_BUILTIN, OUTPUT);

    while (1)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);

        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
    }

    return 0;
}
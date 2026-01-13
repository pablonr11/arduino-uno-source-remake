#include <stdint.h>
#include "arduino_uno.h"
#include "atmega328p.h"
#include "digital.h"

// This is an usage demostration for the digitalRead function
// To run this example we need to connect one side of a button to
// the pin 8 in the Arduino UNO and the other side to GND since
// the pin is configured as INPUT_PULLUP using pinMode.
// While the button is pressed the built-in led in the Arduino UNO
// will blink. If the button is released the led will turn off.
// Since at the time of the creation of this example
// there's no implementation of the delay function
// we use long for loops to waste some time.

digital_value ledStatus = LOW;

int main()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(8, INPUT_PULLUP);

    while (1)
    {

        if (digitalRead(8) == LOW)
        {
            if (ledStatus == HIGH)
            {
                ledStatus = LOW;
            }
            else
            {
                ledStatus = HIGH;
            }

            digitalWrite(LED_BUILTIN, ledStatus);

            for (long i = 0; i < 1000000; i++)
            {
                NO_OPERATION();
            }
        }
        else
        {
            ledStatus = LOW;
            digitalWrite(LED_BUILTIN, ledStatus);
        }
    }

    return 0;
}
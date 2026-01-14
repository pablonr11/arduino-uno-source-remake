#include <stdint.h>
#include <stdbool.h>

#include "arduino_uno.h"
#include "atmega328p.h"
#include "digital.h"
#include "interrupt.h"

// This example shows the usage of attachInterrupt.
// attachInterrupt allows to configure the external interrupt handlers
// in pin 2 and pin 3.
// To run this example we need to connect one side of a button to
// the pin 2 in the Arduino UNO and the other side to GND since
// the pin as INPUT_PULLUP using pinMode.
// In the example we configure pin 2 interrupt in falling edge mode.
// This means the interrupt will be executed when the voltage falls from 5V -> 0v
// in the pin 2.
// So if we press the button the built-in led of the Arduino Uno should start
// blinking. If we press it again it should stop.
// Since at the time of the creation of this example
// there's no implementation of the delay function
// we use long for loops to waste some time.

#define BUTTON_PIN 2

static volatile bool blink = false;
static digital_value ledStatus = LOW;

static void buttonInterrupt(void)
{
    blink = !blink;
}

static void blinkLed()
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

int main()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, FALLING_EDGE);

    // Enable interrupts
    SEI();

    while (1)
    {
        if (blink)
        {
            blinkLed();
        }
        else
        {
            digitalWrite(LED_BUILTIN, LOW);
        }
    }

    return 0;
}
#include <stdint.h>
#include <stdbool.h>

#include "arduino_uno.h"
#include "atmega328p.h"
#include "digital.h"
#include "interrupt.h"
#include "timing.h"

#define BUTTON_PIN 2

static volatile bool blink = false;
static volatile uint8_t buttonPresses = 0;
static digital_value ledStatus = LOW;

static void buttonInterrupt(void)
{
    blink = !blink;
    buttonPresses++;
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
    initTiming();

    bool detachedInterrupt = false;

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, FALLING_EDGE);

    // Enable interrupts
    SEI();

    while (1)
    {
        if (buttonPresses >= 3 && !detachedInterrupt)
        {
            detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
            detachedInterrupt = true;
            blink = false;
        }

        if (blink)
        {
            blinkLed();
        }
        else
        {
            digitalWrite(LED_BUILTIN, LOW);
        }

        // Infinite loop because we don't have anything else to do
        if (detachedInterrupt)
        {
            while (1)
                ;
        }
    }

    return 0;
}
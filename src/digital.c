#include "digital.h"
#include "arduino_uno.h"
#include "atmega328p.h"

/**
 * When digitalWrite or digitalRead is called we need
 * to ensure that PWM is disabled in the specified pin
 * before taking any action.
 *
 * If the pin doesn't support PWM this doesn't need to
 * be called
 */
static void turnOffPWM(uint8_t timer)
{
    switch (timer)
    {
    case T0A:
        TCCR0A &= ~LSHB(COM0A1);
        break;
    case T0B:
        TCCR0A &= ~LSHB(COM0B1);
        break;
    case T1A:
        TCCR1A &= ~LSHB(COM1A1);
        break;
    case T1B:
        TCCR1A &= ~LSHB(COM1B1);
        break;
    case T2A:
        TCCR2A &= ~LSHB(COM2A1);
        break;
    case T2B:
        TCCR2A &= ~LSHB(COM2B1);
        break;
    }
}

void pinMode(uint8_t pin, digital_mode mode)
{
    uint8_t pinPort = pinToPortConversor[pin];
    uint8_t pinBit = pinToBitConversor[pin];

    volatile uint8_t *pinModeRegister = portToModeConversor[pinPort];
    volatile uint8_t *pinOutputRegister = portToOutputConversor[pinPort];

    switch (mode)
    {
    case INPUT:
        *pinModeRegister &= ~pinBit;   // Set DDRx pin bit to 0 (input)
        *pinOutputRegister &= ~pinBit; // Turns off the pull-up resistor for the pin
        break;
    case OUTPUT:
        // Set DDRx pin bit to 1 (output)
        *pinModeRegister |= pinBit;
        break;
    case INPUT_PULLUP:
        *pinModeRegister &= ~pinBit;  // Set DDRx pin bit to 0 (input)
        *pinOutputRegister |= pinBit; // Turn on the pull-up resistor for the pin
        break;
    }
}

void digitalWrite(uint8_t pin, digital_value value)
{
    uint8_t pinPort = pinToPortConversor[pin];
    uint8_t pinBit = pinToBitConversor[pin];
    uint8_t pinTimer = pinToTimerConversor[pin];

    if (pinTimer != NOT_TIMER)
        turnOffPWM(pinTimer);

    volatile uint8_t *pinOutputRegister = portToOutputConversor[pinPort];

    switch (value)
    {
    case LOW:
        *pinOutputRegister &= ~pinBit;
        break;
    case HIGH:
        *pinOutputRegister |= pinBit;
        break;
    }
}

digital_value digitalRead(uint8_t pin)
{
    uint8_t pinPort = pinToPortConversor[pin];
    uint8_t pinBit = pinToBitConversor[pin];
    uint8_t pinTimer = pinToTimerConversor[pin];

    if (pinTimer != NOT_TIMER)
        turnOffPWM(pinTimer);

    volatile uint8_t *pinInputRegister = portToInputConversor[pinPort];

    // Datasheet specifies a nop instruction is needed before reading
    // a value to properly sync, but I think this isn't needed here
    // since we are already doing a bunch of other operations before
    // actually reading the bit value
    // NO_OPERATION()
    if (*pinInputRegister & pinBit)
    {
        return HIGH;
    }

    return LOW;
}

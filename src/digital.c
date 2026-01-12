#include "digital.h"
#include "arduino_uno.h"

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
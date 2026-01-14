#include "interrupt.h"
#include "atmega328p.h"

#include <stdint.h>

void emptyISR(void)
{
}

static voidFunctionP interrupts[EXTERNAL_INTERRUPTS_NUMBER] = {
    emptyISR,
    emptyISR,
};

int8_t digitalPinToInterrupt(uint8_t pin)
{
    if (pin == 2)
    {
        return 0;
    }
    else if (pin == 3)
    {
        return 1;
    }

    return INVALID_PIN_INTERRUPT;
}

void attachInterrupt(int8_t interrupt, voidFunctionP function, external_interrupt_mode mode)
{
    if (interrupt == INVALID_PIN_INTERRUPT || interrupt > EXTERNAL_INTERRUPTS_NUMBER)
    {
        return;
    }

    interrupts[interrupt] = function;

    switch (interrupt)
    {
    case 0:
        EICRA = (EICRA & ~((1 << ISC01) | (1 << ISC00))) | (mode << ISC00); // Sets the mode for pin 2 interrupt activation
        EIMSK |= (1 << INT0);                                               // Enables the interrupt for pin 2
        break;
    case 1:
        EICRA = (EICRA & ~((1 << ISC11) | (1 << ISC10))) | (mode << ISC10); // Sets the mode for pin 3 interrupt activation
        EIMSK |= (1 << INT1);                                               // Enables the interrupt for pin 3
        break;
    }
}

ISR(INT0_vect)
{
    interrupts[0]();
}

ISR(INT1_vect)
{
    interrupts[1]();
}
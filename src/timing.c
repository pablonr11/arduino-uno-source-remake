#include "timing.h";
#include "interrupt.h"
#include "atmega328p.h"

void initTiming(void)
{
    // We ensure that there's no clock source for TCNT0
    // while we do the initial configuration
    TCCR0B = TCCR0B & ~((1 << CS02) | (1 << CS01) | (1 << CS00));
    // We don't want any additional configurations for TCNT0
    // since we are gonna use it in normal mode.
    // NOTE: This probably needs to be changed when PWM is implemented
    TCCR0A = 0;

    // Enable timer overflow interrupt
    TIMSK0 |= (1 << TOIE0);

    SEI(); // Enable global interrupts

    // Enable timer clock and set it's prescaler to 64 division
    TCCR0B |= (TIMER_PRESCALER_64 << CS00);
}

#include "timing.h";
#include "interrupt.h"
#include "atmega328p.h"
#include "utils.h"

#include <stdint.h>

static volatile uint32_t timer0Overflows = 0;

ISR(TIMER0_OVF_vect)
{
    timer0Overflows++;
}

void initTiming(void)
{
    // We ensure that there's no clock source for TCNT0
    // while we do the initial configuration
    TCCR0B = TCCR0B & ~(LSHB(CS02) | LSHB(CS01) | LSHB(CS00));
    // We don't want any additional configurations for TCNT0
    // since we are gonna use it in normal mode.
    // NOTE: This probably needs to be changed when PWM is implemented
    TCCR0A = 0;

    // Enable timer overflow interrupt
    TIMSK0 |= LSHB(TOIE0);

    SEI(); // Enable global interrupts

    // Enable timer clock and set it's prescaler to 64 division
    TCCR0B |= (TIMER_PRESCALER_64 << CS00);
}

uint32_t micros(void)
{
    uint32_t numberOfOverflows;
    uint8_t timerValue;
    uint8_t auxSREG = SREG; // Save SREG value before disabling interrupts.

    CLI(); // Disable interrupts

    // Now since we disabled interrupts timer0Overflows won't be incremented
    // So we have to check manually if an overflow occurs.
    // Until we restore SREG

    numberOfOverflows = timer0Overflows;
    timerValue = TCNT0;

    // If the overflow flag is set this means and overflow happened
    // after disabling interrupts.
    // Also, here we need to check if the timer value is 255.
    // Since the flag is set once the timer reaches 255
    // we would count 2 overflows instead of 1
    if (TIFR0 & LSHB(TOV0) && timerValue < 255)
    {
        numberOfOverflows++;
        // timer0Overflows will be increased once we reenable interrupts
    }

    // This should enable interrupts again. Unless they were disabled before calling this function
    SREG = auxSREG;

    return (numberOfOverflows * MICROSECONDS_PER_TIMER0_OVERFLOW + timerValue * TIMER0_TICK_MICROSECONDS);
}
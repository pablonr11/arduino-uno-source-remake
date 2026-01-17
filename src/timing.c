#include "timing.h";
#include "interrupt.h"
#include "atmega328p.h"
#include "utils.h"

#include <stdint.h>

static volatile uint32_t timer0Overflows = 0;
static volatile uint32_t timer0Millis = 0;
static volatile uint16_t timer0LostMillisFraction = 0;

ISR(TIMER0_OVF_vect)
{
    uint32_t milliseconds = timer0Millis;
    uint16_t millisFract = timer0LostMillisFraction;

    // Here we increment milliseconds by 1 everytime.
    // Technically it should be 1.024 but the value gets
    // truncated to 1 because we are storing an integer.
    milliseconds += MILLISECONDS_PER_TIMER0_OVERFLOW;
    // We can account for the lost part storing the fractional part
    // and adding an extra millisecond once we stored enough
    // Store 24 lost microseconds
    millisFract += MILLISECONDS_FRACTION_INCREMENT;

    // Once we stored more than 1000 microseconds add them to the milliseconds
    if (millisFract >= MICROSECONDS_PER_MILLISECOND)
    {
        millisFract -= MICROSECONDS_PER_MILLISECOND;
        milliseconds += 1;
    }

    timer0Millis = milliseconds;
    timer0LostMillisFraction = millisFract;
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

void delayMicroseconds(uint16_t microseconds)
{
    // This function assumes -Os optimization

#if F_CPU == 16000000
    // If this function is called for a 1 microsecond delay we can return imediately
    // The explanitation for this behaviur is the following:
    // The assembly code when we call delayMicroseconds is:
    // ldi r24 0x01 // 1 cycle (initializes microseconds variable)
    // ldi r25 0x00 // 1 cycle (initializes microseconds variable)
    // call delayMicroseconds // 4 cycles
    // --- Inside delayMicroseconds ---
    // sbiw r25 0x02 // 2 cycles (substracts 2 from microseconds. If the value is -1 the carry flag is set)
    // brcs _ret_addr // 2 cyles (jumps to ret if the carry flag is set)
    // ret // 4 cycles
    // ---------------------------------
    // This is the result for calling delayMicroseconds(1).
    // If we call delayMicroseconds(variable) (using a variable for the microseconds)
    // the first two ldi instruction will be replaced with LDD or LDS instructions
    // which consumes 2 instructions each.
    // So 2 cycles (or 4) + 4 + 2 + 2 + 4 = 14 (or 16) cycles
    // Since our MCU should be running at 16MHz this means 16 cycles are 1 microsecond.
    // ATMega328P can be configured to run with other frequencies. For those
    // Another values or additional nop instructions should be needed.
    // Also brcs is 2 cycles if the condition is true. Otherwise it won't
    // jump to ret and it will only be 1 cycle.
    if (microseconds <= 1)
        return;

    // At this point we spent 13 or 15 cycles

    // We need to multiply by 4 microseconds because the loop takes 4 cycles each loop
    microseconds *= 4; // This takes 4 cycles (2 add and 2 adc instructions)

    // At this point we spent 17 or 19 cycles so we can substract 5
    // to compensate. This is 20 cycles less for the loop
    microseconds -= 5; // Also this operation takes 2 cycles to compensates for the extra cycles we are removing.
#else
#warning "delayMicroseconds not implemented for F_CPU value"
#endif

    // This line was a bit hard to understand...
    // The output of this will be something like:
    // loop:
    //      sbiw r24, 0x01
    //      brne loop
    // This is just a loop to waste cycles.
    // sbiw substracts 1 from microseconds
    // brne jumps to loop if the previous operation wasn't 0.
    // : "+w" (microseconds) is an inline assembly constraint
    // that indicates that the inserted assembly code works as
    // as an input/output for the registers related to the microseconds
    // variable.
    // +: Indicates an output operand that is also an input.
    // w: Use a register compatible with ADIW and SBIW instructions
    // https://gcc.gnu.org/wiki/avr-gcc#Inline_Assembly
    // https://avrdudes.github.io/avr-libc/avr-libc-user-manual-2.2.0/inline_asm.html#inline_asm_constraints
    // -----------------------------------
    // sbiw consumes 2 cycles and brne consumes 2 cycles when loops back and 1 cycle when it exits the loop
    // This means the loop takes 4 cycles
    __asm__ __volatile__(
        "1: sbiw %0,1\n\t"
        "brne 1b" : "+w"(microseconds));

    // At this point is possible to be some cycles off of the exact value
    // Probably this is the closest we can get without using another timer/counter
    // from the mcu.
    // More comments than code lmao
}

uint32_t millis(void)
{
    uint32_t milliseconds;
    uint8_t auxSREG = SREG; // Store status register

    // Here we need to disable interrupts before reading timer0Millis
    // because it is 32 bits.
    // If we don't disable interrupts, an overflow interrupt can happen
    // between the instruction that load the value
    // causing that we read an incorrect value.
    CLI();
    milliseconds = timer0Millis;

    // Restore SREG. (This should enable interrupts if they weren't disabled before)
    SREG = auxSREG;

    return milliseconds;
}
#ifndef _TIMING_H
#define _TIMING_H

#include "atmega328p.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

// 1s = 1000ms = 1000000microseconds = 16000000 clock cycles in Arduino UNO
#define CLOCK_CYCLES_PER_MICROSECOND (F_CPU / 1000000) // 16 clock cycles in a microsecond
#define TIMER0_PRESCALER 64
#define TIMER0_TICK_MICROSECONDS (TIMER0_PRESCALER / CLOCK_CYCLES_PER_MICROSECOND)

// Since the prescaler is set to 64 and TCNT0 is an 8 bit counter
// 64 * 256 gives the amount of cycles per timer0 overclock. (16384)
// So (64 * 256) / CLOCK_CYCLES_PER_MICROSECOND = 1024 microseconds per timer0 overclock
#define MICROSECONDS_PER_TIMER0_OVERFLOW (TIMER0_PRESCALER * 256 / CLOCK_CYCLES_PER_MICROSECOND)
#define MILLISECONDS_PER_TIMER0_OVERFLOW (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define MICROSECONDS_PER_MILLISECOND 1000
#define MILLISECONDS_FRACTION_INCREMENT (MICROSECONDS_PER_TIMER0_OVERFLOW - MICROSECONDS_PER_MILLISECOND)

    /**
     * @brief This function should be called at the start of the program
     * to make sure the timing functions work properly.
     */
    void initTiming(void);

    /**
     * @brief Calculates the amount of microseconds since timer/counter0 was enabled
     * @return The amount of microseconds since timer/counter0 was enabled
     */
    uint32_t micros(void);

    /**
     * @brief Delays for the given amount of microseconds
     * @param microseconds The amount of microseconds to delay.
     * For delays higher than 16383 microseconds use delay()
     */
    void delayMicroseconds(uint16_t microseconds);

    /**
     * @brief Calculates the amount of milliseconds since timer/counter0 was enabled
     * @return The amount of milliseconds since timer/counter0 was enabled
     */
    uint32_t millis(void);

    /**
     * @brief Delays for the given amount of milliseconds
     * @param milliseconds The amount of milliseconds to delay
     */
    void delay(uint32_t milliseconds);

#ifdef __cplusplus
}
#endif

#endif
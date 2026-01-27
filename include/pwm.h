#ifndef _PWM_H
#define _PWM_H

/**
 * @brief This function is used by initTiming to configure
 * timer0 pwm before starting the timer. Technicaly this
 * logic could be written in initTiming, but for maintaining
 * all PWM related code in one place we define this function.
 */
static inline void timer0ConfigPwm(void)
{
    TCCR0A |= (LSHB(WGM01) | LSHB(WGM00)); // Set Fast PWM mode
    TCCR0B &= ~LSHB(WGM02);                // Ensure WGM02 is 0 for Fast PWM mode
}

/**
 * @brief Configures the 3 timers for PWM.
 * Timer 0 is configured in fast PWM mode
 * Timers 1 and 2 are configured in phase correct PWM mode.
 * We need to call this function before being able to use analogWrite
 * Timer 0 is configured in fast PWM mode because if we use
 * phase correct timing functions will break since we reach MAX value
 * half as often. (0 -> MAX -> 0)
 */
void initPWM(void);

#endif
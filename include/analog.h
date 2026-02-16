#ifndef _ANALOG_H
#define _ANALOG_H

#include "arduino_uno.h"
#include "atmega328p.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define DEFAULT_ANALOG_REFERENCE 1  // Internal AVcc reference
#define EXTERNAL_ANALOG_REFERENCE 0 // External AREF pin reference
#define INTERNAL_ANALOG_REFERENCE 3 // Internal 1.1V reference

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

    void initAnalog(void);

    /**
     * @brief sets de analog reference for the ADC
     * @param ref The analog reference to set
     *            Valid values are:
     *              - DEFAULT_ANALOG_REFERENCE (default)
     *              - EXTERNAL_ANALOG_REFERENCE
     *              - INTERNAL_ANALOG_REFERENCE
     */
    void analogReference(uint8_t ref);

    int16_t analogRead(analog_pin pin);

    /**
     * @brief Sets the analog output of a pin using PWM.
     * This function changes the pin to OUTPUT. Calling
     * pinMode is not needed.
     * @param pin The pin to apply the value. Not all pins support analogWrite.
     * Only the ones with ~.
     * @param value The value for the pin (0-255)
     */
    void analogWrite(uint8_t pin, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
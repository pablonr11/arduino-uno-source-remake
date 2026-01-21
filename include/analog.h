#ifndef _ANALOG_H
#define _ANALOG_H

#include "arduino_uno.h"

#include <stdint.h>

#define DEFAULT_ANALOG_REFERENCE 1  // Internal AVcc reference
#define EXTERNAL_ANALOG_REFERENCE 0 // External AREF pin reference
#define INTERNAL_ANALOG_REFERENCE 3 // Internal 1.1V reference

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

#endif
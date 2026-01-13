#ifndef _DIGITAL_H
#define _DIGITAL_H

#include <stdint.h>

typedef enum
{
    LOW,
    HIGH,
} digital_value;

typedef enum
{
    INPUT,
    OUTPUT,
    INPUT_PULLUP
} digital_mode;

void pinMode(uint8_t pin, digital_mode mode);

void digitalWrite(uint8_t pin, digital_value value);

digital_value digitalRead(uint8_t pin);

#endif
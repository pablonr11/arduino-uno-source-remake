#ifndef _DIGITAL_H
#define _DIGITAL_H

#include <stdint.h>

#define LOW 0
#define HIGH 1

typedef enum
{
    INPUT,
    OUTPUT,
    INPUT_PULLUP
} digital_mode;

void pinMode(uint8_t pin, digital_mode mode);

#endif
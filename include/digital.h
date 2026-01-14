#ifndef _DIGITAL_H
#define _DIGITAL_H

#include <stdint.h>

// Available digital pin I/O values
typedef enum
{
    LOW,
    HIGH,
} digital_value;

// Pin modes
typedef enum
{
    INPUT,
    OUTPUT,
    INPUT_PULLUP
} digital_mode;

/**
 * @brief Used to specify the direction of a pin
 * @param pin The pin number for the Arduino UNO
 * @param mode The data direction for the specified pin
 */
void pinMode(uint8_t pin, digital_mode mode);

/**
 * @brief Specifies the value for an output pin.
 * @param pin The pin to apply the value. It's important to set the pin as an OUTPUT before calling digitalWrite
 * @param value The value for the pin
 */
void digitalWrite(uint8_t pin, digital_value value);

/**
 * @brief Read the value for an input pin
 * @param pin The pin to read froml. It's important to set the pin as an INPUT or INPUT_PULLUP before calling digitalRead
 * @return The value obtained from the pin
 */
digital_value digitalRead(uint8_t pin);

#endif
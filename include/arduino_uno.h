#ifndef _ARDUINO_H
#define _ARDUINO_H

#include <stdint.h>

#define LED_BUILTIN 13

typedef enum
{
    // Arduino UNO R3 analog inputs
    ADC_PIN0 = 0,
    ADC_PIN1,
    ADC_PIN2,
    ADC_PIN3,
    ADC_PIN4,
    ADC_PIN5,
    // Additional ATMega328P analog inputs
    ADC_PIN6,
    ADC_PIN7,
    ADC_TEMPERATURE,
    ADC_1V1 = 0b1110, // 14
    ADC_GND,
} analog_pin;

// Port indexes
#define PB 0
#define PC 1
#define PD 2

// Timer indexes
#define NOT_TIMER 0
#define T0A 1
#define T0B 2
#define T1A 3
#define T1B 4
#define T2A 5
#define T2B 6

// I2C pins
#define SDA (18)
#define SCL (19)

// For example, if we want to know the port for pin 13 in the Arduino UNO
// We can get pinToBitConversor[13] (LSHB(5) aka. Fifth bit)
// and pinToPortConversor[13] (PB aka. 3. This value can be mapped to a
// register using portToXConversor arrays)
extern const uint8_t pinToBitConversor[];
extern const uint8_t pinToPortConversor[];
extern const uint8_t pinToTimerConversor[];

extern volatile uint8_t *const portToModeConversor[];
extern volatile uint8_t *const portToOutputConversor[];
extern volatile uint8_t *const portToInputConversor[];

#endif
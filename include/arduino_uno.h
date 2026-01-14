#ifndef _ARDUINO_H
#define _ARDUINO_H

#include <stdint.h>

#define LED_BUILTIN 13

// Port indexes
#define PB 0
#define PC 1
#define PD 2

// For example, if we want to know the port for pin 13 in the Arduino UNO
// We can get pinToBitConversor[13] (LSHB(5) aka. Fifth bit)
// and pinToPortConversor[13] (PB aka. 3. This value can be mapped to a
// register using portToXConversor arrays)
extern const uint8_t pinToBitConversor[];
extern const uint8_t pinToPortConversor[];

extern volatile uint8_t *const portToModeConversor[];
extern volatile uint8_t *const portToOutputConversor[];
extern volatile uint8_t *const portToInputConversor[];

#endif
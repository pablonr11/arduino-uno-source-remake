#ifndef _ATMEGA328P_H
#define _ATMEGA328P_H

#include "utils.h"

// By default the Arduino UNO runs at 16MHz.
// It could run at another frequency if it is configured that way.
// In that case this define need to be changed or overwritten.
#ifndef F_CPU
#define F_CPU 16000000
#endif

// Pin output registers
#define PORTB REG8(0x25)
#define PORTC REG8(0X28)
#define PORTD REG8(0x2B)

// Pin Data direction registers
#define DDRB REG8(0x24)
#define DDRC REG8(0x27)
#define DDRD REG8(0x2A)

// Pin input registers
#define PINB REG8(0x23)
#define PINC REG8(0x26)
#define PIND REG8(0x29)

#endif
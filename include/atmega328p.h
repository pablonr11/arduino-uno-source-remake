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

// --- Interrupt registers ---

// Interrupt control for pins 2 and 3
#define EICRA REG8(0x69) // External Interrupt Control Register A
#define ISC00 0
#define ISC01 1
#define ISC10 2
#define ISC11 3

#define EIMSK REG8(0x3D) // External Interrupt Mask Register
#define INT0 0
#define INT1 1

#define EIFR REG8(0X3C) // External Interrupt Flag Register
#define INTF0 0
#define INTF1 1

// Pin change interrupts
#define PCICR REG8(0X68) // Pin Change Interrupt Control Register
#define PCIE0 0
#define PCIE1 1
#define PCIE2 2

#define PCIFR REG8(0X3B) // Pin Change Interrupt Flag Register
#define PCIF0 0
#define PCIF1 1
#define PCIF2 2

// This registers control which pins should trigger the interrupt for pin changes
#define PCMSK2 REG8(0X6D) // Pin Change Mask Register 2
#define PCINT16 0
#define PCINT17 1
#define PCINT18 2
#define PCINT19 3
#define PCINT20 4
#define PCINT21 5
#define PCINT22 6
#define PCINT23 7

#define PCMSK1 REG8(0X6C) // Pin Change Mask Register 1
#define PCINT8 0
#define PCINT9 1
#define PCINT10 2
#define PCINT11 3
#define PCINT12 4
#define PCINT13 5
#define PCINT14 6

#define PCMSK0 REG8(0X6B) // Pin Change Mask Register 0
#define PCINT0 0
#define PCINT1 1
#define PCINT2 2
#define PCINT3 3
#define PCINT4 4
#define PCINT5 5
#define PCINT6 6
#define PCINT7 7

#endif
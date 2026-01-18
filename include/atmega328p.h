#ifndef _ATMEGA328P_H
#define _ATMEGA328P_H

#include "utils.h"

// By default the Arduino UNO runs at 16MHz.
// It could run at another frequency if it is configured that way.
// In that case this define need to be changed or overwritten.
#ifndef F_CPU
#define F_CPU 16000000
#endif

// Status register
#define SREG REG8(0x5F)
#define SREG_C 0
#define SREG_Z 1
#define SREG_N 2
#define SREG_V 3
#define SREG_S 4
#define SREG_H 5
#define SREG_T 6
#define SREG_I 7

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

// --- Timer/Counter Registers ---
#define TCCR0A REG8(0x44) // Timer/Counter Control Register A
#define WGM00 0
#define WGM01 1
#define COM0B0 4
#define COM0B1 5
#define COM0A0 6
#define COM0A1 7

#define TCCR0B REG8(0x45) // Timer/Counter Control Register B
#define CS00 0
#define CS01 1
#define CS02 2
#define WGM02 3
#define FOC0B 6
#define FOC0A 7

#define TCNT0 REG8(0X46) // Timer/Counter0 Value
#define OCR0A REG8(0x47) // Output Compare Register A. Stores compare value A for timer/counter0
#define OCR0B REG8(0x48) // Output Compare Register B. Stores compare value B for timer/counter0

#define TIMSK0 REG8(0x6E) // Timer/Counter Interrupt Mask Register. Enable/Disable timer/counter0 interrupts
#define TOIE0 0
#define OCIE0A 1
#define OCIE0B 2

#define TIFR0 REG8(0X35) // Timer/Counter0 Interrupt Flag Register.
#define TOV0 0
#define OCF0A 1
#define OCF0B 2

#define TIMER_PRESCALER_8 0b010
#define TIMER_PRESCALER_64 0b011
#define TIMER_PRESCALER_256 0b100
#define TIMER_PRESCALER_1024 0b101

//--- Serial registers ---
#define UDR0 REG8(0xC6)   // USART I/O Data Register.
#define UCSR0A REG8(0xC0) // USART Control and Status Register A
#define MPCM0 0
#define U2X0 1
#define UPE0 2
#define DOR0 3
#define FE0 4
#define UDRE0 5
#define TXC0 6
#define RXC0 7

#define UCSR0B REG8(0xC1) // USART Control and Status Register B
#define TXB80 0
#define RXB80 1
#define UCSZ02 2
#define TXEN0 3
#define RXEN0 4
#define UDRIE0 5
#define TXCIE0 6
#define RXCIE0 7

#define UCSR0C REG8(0xC2) // USART Control and Status Register C
#define UCPOL0 0
#define UCSZ00 1
#define UCSZ01 2
#define USBS0 3
#define UPM00 4
#define UPM01 5
#define UMSEL00 6
#define UMSEL01 7

#define UBRR0H REG8(0xC5) // USART Baud Rate Register (MSB)
#define UBRR0L REG8(0xC4) // USART Baud Rate Register (LSB)

#endif
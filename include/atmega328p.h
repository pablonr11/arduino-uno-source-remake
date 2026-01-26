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

// --- Timer/Counter0 Registers ---
#define TCCR0A REG8(0x44) // Timer/Counter0 Control Register A
#define WGM00 0
#define WGM01 1
#define COM0B0 4
#define COM0B1 5
#define COM0A0 6
#define COM0A1 7

#define TCCR0B REG8(0x45) // Timer/Counter0 Control Register B
#define CS00 0
#define CS01 1
#define CS02 2
#define WGM02 3
#define FOC0B 6
#define FOC0A 7

#define TCNT0 REG8(0X46) // Timer/Counter0 Value
#define OCR0A REG8(0x47) // Output Compare Register A. Stores compare value A for timer/counter0
#define OCR0B REG8(0x48) // Output Compare Register B. Stores compare value B for timer/counter0

#define TIMSK0 REG8(0x6E) // Timer/Counter0 Interrupt Mask Register. Enable/Disable timer/counter0 interrupts
#define TOIE0 0
#define OCIE0A 1
#define OCIE0B 2

#define TIFR0 REG8(0X35) // Timer/Counter0 Interrupt Flag Register.
#define TOV0 0
#define OCF0A 1
#define OCF0B 2

// --- Timer/Counter1 Registers ---
#define TCCR1A REG8(0x80) // Timer/Counter1 Control Register A
#define WGM10 0
#define WGM11 1
#define COM1B0 4
#define COM1B1 5
#define COM1A0 6
#define COM1A1 7

#define TCCR1B REG8(0x81) // Timer/Counter1 Control Register B
#define CS10 0
#define CS11 1
#define CS12 2
#define WGM12 3
#define WGM13 4
#define ICES1 6
#define ICES2 7

#define TCCR1C REG8(0x82) // Timer/Counter1 Control Register C
#define FOC1B 6
#define FOC1A 7

#define TCNT1 REG16(0x84) // Timer/Counter1
#define TCNT1H REG8(0x85) // Higher Timer/Counter1 Bits
#define TCNT1L REG8(0x84) // Lower Timer/Counter1 Bits

#define OCR1A REG16(0x88) // Output Compare Register 1 A
#define OCR1AH REG8(0x89) // Higher Compare Resgister 1 A Bits
#define OCR1AL REG8(0x88) // Lower Compare Resgister 1 A Bits

#define OCR1B REG16(0x8A) // Output Compare Register 1 B
#define OCR1BH REG8(0x8B) // Higher Compare Resgister 1 B Bits
#define OCR1BL REG8(0x8A) // Lower Compare Resgister 1 B Bits

#define TIMSK1 REG8(0x6F) // Timer/Counter1 Interrupt Mask Register
#define TOIE1 0
#define OCIE1A 1
#define OCIE1B 2
#define ICIE1 5

#define TIFR1 REG8(0x36) // Timer/Counter1 Interrupt Flag Register
#define TOV1 0
#define OCF1A 1
#define OCF1B 2
#define ICF1 5

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

// --- ADC Registers ---
#define ADMUX REG8(0x7C) // ADC Multiplexer Selection Register
#define MUX0 0
#define MUX1 1
#define MUX2 2
#define MUX3 3
#define ADLAR 5
#define REFS0 6
#define REFS1 7

#define ADCSRA REG8(0x7A) // ADC Control and Status Register A
#define ADPS0 0
#define ADPS1 1
#define ADPS2 2 // Preescaler select bits
#define ADIE 3
#define ADIF 4
#define ADATE 5
#define ADSC 6 // Set to 1 to start a conversion
#define ADEN 7 // ADC Enable. Write to 1 at initialization

// NOTE 1: The first conversion after setting ADSC on ADEN (or both at the same time)
// takes longer (25 cycles) than usual (13 cycles)

// NOTE 2: Since we are going to use the 10 bits in the data register
// we need to set out preescaler to 128 (all ADPS bits to 1) if we are
// running at 16MHz.

#define ADCSRB REG8(0x7B) // ADC Control and Status Register B
#define ADTS0 0
#define ADTS1 1
#define ADTS2 2
#define ACME 6 // This bit appears in the datasheet but is not specified what it does.

#define DIDR0 REG8(0x7E) // Digital Input Disable Register 0
#define ADC0D 0
#define ADC1D 1
#define ADC2D 2
#define ADC3D 3
#define ADC4D 4
#define ADC5D 5

#define ADCL REG8(0x78) // Lower ADC Data Register
#define ADCH REG8(0x79) // Higher ADC Data Register

#endif
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "utils.h"

#include <stdint.h>

// Enable interrupts setting the global interrupt mask.
#define SEI() __asm__("sei")
// Disable interrupts setting the global interrupt mask.
#define CLI() __asm__("cli")

// This is basically the same as the original source
// - signal indicates it is an interrupt routine so the compiler includes the RETI instruction at the end
// - used indicates the compiler to not remove the function even if there's direct reference to it
#define ISR(ivector)                                                      \
    void ivector(void) __attribute__((signal, used, externally_visible)); \
    void ivector(void)

#define VECTOR(vector_number) __vector_##vector_number

// Vector names are defined in crtm328p.o file.
// This file is automatically added to the compilation by the linker.
// If we don't define an ISR for a vector it will point to __bad_interrupt
// from the crt file.

// Interrupt vectors. Those are the parameters expected by the ISR macro
#define INT0_vect VECTOR(1)
#define INT1_vect VECTOR(2)
#define PCINT0_vect VECTOR(3)
#define PCINT1_vect VECTOR(4)
#define PCINT2_vect VECTOR(5)
#define WDT_vect VECTOR(6)
#define TIMER2_COMPA_vect VECTOR(7)
#define TIMER2_COMPB_vect VECTOR(8)
#define TIMER2_OVF_vect VECTOR(9)
#define TIMER1_CAPT_vect VECTOR(10)
#define TIMER1_COMPA_vect VECTOR(11)
#define TIMER1_COMPB_vect VECTOR(12)
#define TIMER1_OVF_vect VECTOR(13)
#define TIMER0_COMPA_vect VECTOR(14)
#define TIMER0_COMPB_vect VECTOR(15)
#define TIMER0_OVF_vect VECTOR(16)
#define SPI_STC_vect VECTOR(17)
#define USART_RX_vect VECTOR(18)
#define USART_UDRE_vect VECTOR(19)
#define USART_TX_vect VECTOR(20)
#define ADC_vect VECTOR(21)
#define EE_READY_vect VECTOR(22)
#define ANALOG_COMP_vect VECTOR(23)
#define TWI_vect VECTOR(24)
#define SPM_READY_vect VECTOR(25)

#define INVALID_PIN_INTERRUPT -1
#define EXTERNAL_INTERRUPTS_NUMBER 2 // Specific number of external interrupts.

typedef enum
{
    LOW_LEVEL = 0,
    ANY,
    FALLING_EDGE,
    RISING_EDGE
} external_interrupt_mode;

/**
 * @brief Converts a digital pin to the interrupt index used by attachInterrupt and detachInterrupt
 * @param pin The pin to enable the interrupt. (Accepted pins are 2 and 3)
 * @return The interrupt index used by attachInterrupt and detachInterrupt.
 * If pin is not a valid value INVALID_PIN_INTERRUPT is returned instead
 *
 * @note Technically Arduino UNO can throw an interrupt for all digital pins,
 * but those interrupts are for the entire port. (PCINT0, PCINT1 and PCINT2)
 * The only pins that have its own interrupts are pin 2 and 3 (INT0 and INT1)
 */
int8_t digitalPinToInterrupt(uint8_t pin);

/**
 * @brief Use this function to configure interrupts for pins 2 and 3
 * @param interrupt Interrupt index to configure.
 * Use digitalPinToInterrupt to convert the pin to its corresponding index
 * @param function The function to execute when the interrupt is executed
 * @param mode When the interrupt request should be made
 */
void attachInterrupt(int8_t interrupt, voidFunctionP function, external_interrupt_mode mode);

/**
 * @brief Use this function to turn off an interrupt.
 * @param interrupt Interrupt index to configure.
 * Use digitalPinToInterrupt to convert the pin to its corresponding index
 */
void detachInterrupt(int8_t interrupt);

#endif
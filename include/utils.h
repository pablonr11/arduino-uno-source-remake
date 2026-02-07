#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

// Access an 8 bit register
#define REG8(addr) (*(volatile uint8_t *)(addr))
// Access a 16 bit register
#define REG16(addr) (*(volatile uint16_t *)(addr))
// Left Shift 1 bit
#define LSHB(bit) (1 << bit)
// Inserts one "nop" instruction
#define NO_OPERATION() __asm__("nop")

// Converts a decimal value into a BCD value
#define DEC_TO_BCD(val) (((val / 10) << 4) | (val % 10))
// Converts a BCD value into a decimal value
#define BCD_TO_DEC(val) (((val >> 4) * 10) + (val % 16))

// Void function pointer type
typedef void (*voidFunctionP)(void);

#endif
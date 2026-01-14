#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

// Access an 8 bit address
#define REG8(addr) (*(volatile uint8_t *)(addr))
// Left Shift 1 bit
#define LSHB(bit) (1 << bit)
// Inserts one "nop" instruction
#define NO_OPERATION() __asm__("nop")

#endif
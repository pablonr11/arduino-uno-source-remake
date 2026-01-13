#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

#define REG8(addr) (*(volatile uint8_t *)(addr))
#define LSHB(bit) (1 << bit) // Left Shift 1 bit
#define NO_OPERATION() __asm__("nop")

#endif
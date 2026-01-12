#include "arduino_uno.h"
#include "utils.h"
#include "atmega328p.h"

#include <stdint.h>

const uint8_t pinToBitConversor[] = {
    // Port D
    LSHB(0),
    LSHB(1),
    LSHB(2),
    LSHB(3),
    LSHB(4),
    LSHB(5),
    LSHB(6),
    LSHB(7),
    // Port B
    LSHB(0),
    LSHB(1),
    LSHB(2),
    LSHB(3),
    LSHB(4),
    LSHB(5),
    // Port C
    LSHB(0),
    LSHB(1),
    LSHB(2),
    LSHB(3),
    LSHB(4),
    LSHB(5),
};

const uint8_t pinToPortConversor[] = {
    // Port D
    PD,
    PD,
    PD,
    PD,
    PD,
    PD,
    PD,
    PD,
    // Port B
    PB,
    PB,
    PB,
    PB,
    PB,
    PB,
    // Port C
    PC,
    PC,
    PC,
    PC,
    PC,
    PC,
};

volatile uint8_t *const portToModeConversor[] = {
    &DDRB,
    &DDRC,
    &DDRD};

volatile uint8_t *const portToOutputConversor[] = {
    &PORTB,
    &PORTC,
    &PORTD,
};

volatile uint8_t *const portToInputConversor[] = {
    &PINB,
    &PINC,
    &PIND,
};
#include "arduino_uno.h"
#include "utils.h"
#include "atmega328p.h"

#include <stdint.h>

// Used to convert a pin number to its corresponding bit
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

// Used to get the port of a pin
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

const uint8_t pinToTimerConversor[] = {
    NOT_TIMER,
    NOT_TIMER,
    NOT_TIMER,
    T2B,
    NOT_TIMER,
    T0B,
    T0A,
    NOT_TIMER,
    NOT_TIMER,
    T1A,
    T1B,
    T2A,
    NOT_TIMER,
    NOT_TIMER,
    NOT_TIMER,
    NOT_TIMER,
    NOT_TIMER,
    NOT_TIMER,
    NOT_TIMER,
    NOT_TIMER,
};

// Converts a port to its corresponding data direction register
volatile uint8_t *const portToModeConversor[] = {
    &DDRB,
    &DDRC,
    &DDRD,
};

// Converts a port to its corresponding output register
volatile uint8_t *const portToOutputConversor[] = {
    &PORTB,
    &PORTC,
    &PORTD,
};

// Converts a port to its corresponding input register
volatile uint8_t *const portToInputConversor[] = {
    &PINB,
    &PINC,
    &PIND,
};
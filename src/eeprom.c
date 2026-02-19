#include "atmega328p.h"
#include "interrupt.h"
#include "utils.h"
#include "eeprom.h"

#include <stdint.h>

uint8_t eeprom_read_byte(uint16_t address)
{
    // First we wait until EEPE is 0.
    // This is required to ensure that previous operations
    // finished correctly
    while (EECR & LSHB(EEPE))
        ;

    // Write the addres we want to write to
    // Bits 15-10 are reserved, so we set them
    // to 0 anding with 0x3FF
    EEAR = (address & 0x3FF);

    // Start EEPROM read setting EERE to 1
    EECR |= LSHB(EERE);

    return EEDR;
}

void eeprom_write_byte(uint16_t address, uint8_t value)
{
    uint8_t oldSREG;
    // First we wait until EEPE is 0.
    // This is required to ensure that previous operations
    // finished correctly
    while (EECR & LSHB(EEPE))
        ;

    // Here we could wait until SPMEN becomes 0.
    // This shouldn't be needed because the arduino bootlader
    // is not using this function at all

    // Write the addres we want to write to
    // Bits 15-10 are reserved, so we set them
    // to 0 anding with 0x3FF
    EEAR = (address & 0x3FF);

    // Set the value we want to write
    EEDR = value;

    // Before prociding we save the SREG register
    // because the next 2 steps need to be atomic
    // and for that we are going to disable interrupts
    oldSREG = SREG;
    CLI(); // Disable interrupts

    // Write 1 to EEMPE
    EECR |= LSHB(EEMPE);
    // Within 4 clock cycles we write 1 TO EEPE
    EECR |= LSHB(EEPE);

    // Restore SREG
    SREG = oldSREG;
}

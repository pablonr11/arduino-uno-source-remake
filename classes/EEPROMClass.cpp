#include "EEPROMClass.h"

#include <stdint.h>

uint8_t EEPROMClass::read(uint16_t address)
{
    return eeprom_read_byte(address);
}

void EEPROMClass::write(uint16_t address, uint8_t value)
{
    eeprom_write_byte(address, value);
}

void EEPROMClass::update(uint16_t address, uint8_t value)
{
    if (value != read(address))
    {
        write(address, value);
    }
}

EEPROMClass EEPROM;
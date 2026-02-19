#ifndef _EEPROM_H
#define _EEPROM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define EEPROM_LENGTH 1024

    /**
     * @brief Read a byte from the EEPROM
     * @param address The address to read from
     * @return The read byte
     */
    uint8_t eeprom_read_byte(uint16_t address);

    /**
     * @brief Writes a byte to the EEPROM
     * @param address The address to write to
     * @param value The value to write
     */
    void eeprom_write_byte(uint16_t address, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
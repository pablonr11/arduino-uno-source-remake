#ifndef _EEPROM_CLASS_H
#define _EEPROM_CLASS_H

#include "eeprom.h"

#include <stdint.h>

// In C++ structs and classes are basically the same.
// Since everything is public by default in a struct
// and we are not going to use privates we use a struct
// instead of a class

struct EEPROMClass
{
    /**
     * @brief Read a byte from the EEPROM
     * @param address The address to read from
     * @return The read value
     */
    uint8_t read(uint16_t);

    /**
     * @brief Writes a byte to the EEPROM
     * @param address The address to write to
     * @param value The value to write
     */
    void write(uint16_t, uint8_t);

    /**
     * @brief Write a byte to the EEPROM only if the value changes
     * Internally this call read to check the value and if it's
     * not the same value does the write
     * @param address The address to update
     * @param value The value to write
     */
    void update(uint16_t, uint8_t);

    /**
     * @brief Returns the length of the EEPROM
     * @return The length of the EEPROM
     */
    uint16_t length()
    {
        return EEPROM_LENGTH;
    }

    /**
     * @brief Puts the indicated variable in the EEPROM.
     * The variable can be of any length. Even structs.
     * @param address The address to put the data
     * @param t The variable containing the data to put
     * @return a reference to the written data
     */
    template <typename T>
    const T &put(uint16_t address, const T &t)
    {
        const uint8_t *ptr = (const uint8_t *)&t;
        for (uint16_t i = 0; i < sizeof(T); i++)
        {
            update(address + i, ptr[i]);
        }

        return t;
    }

    /**
     * @brief Gets the indicated variable length from the EEPROM
     * The variable can be of any length. Even structs
     * @param address The address to get the data from
     * @param t The variable where the read data will be stored
     * @return a reference to the data
     */
    template <typename T>
    T &get(uint16_t address, T &t)
    {
        uint8_t *ptr = (uint8_t *)&t;
        for (uint16_t i = 0; i < sizeof(T); i++)
        {
            ptr[i] = read(address + i);
        }

        return t;
    }
};

extern EEPROMClass EEPROM;

#endif
#include "timing.h"
#include "serial.h"
#include "eeprom.h"

#include <stdint.h>
#include <stdlib.h>

// This example uses the eeprom to store the amount
// of resets.
// While running this program, every time we reset
// the device, at the start of the program we read
// the first 2 bytes from the eeprom and store the
// read bytes in a uint16_t variable. Then we increase
// the variable and write the new value to the eeprom.
// Then we print the value through serial.

/**
 * @brief Gets the number of device resets
 * increments the number and stores it in the EEPROM
 * @return The number of resets
 */
uint16_t countReset();

int main()
{
    char resetsString[6];
    uint16_t resets = 0;

    initTiming();
    serialBegin(9600);

    resets = countReset();
    utoa(resets, resetsString, 10);

    serialPrint("Number of resets: ");
    serialPrintln(resetsString);

    while (1)
    {
    }

    return 0;
}

uint16_t countReset()
{
    uint16_t counter;

    counter = eeprom_read_byte(0);
    counter |= ((uint16_t)eeprom_read_byte(1)) << 8;

    counter++;

    eeprom_write_byte(0, (uint8_t)(counter & 0xFF));
    eeprom_write_byte(1, (uint8_t)((counter >> 8) & 0xFF));

    return counter;
}
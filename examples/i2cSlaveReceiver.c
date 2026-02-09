#include "timing.h"
#include "serial.h"
#include "interrupt.h"
#include "i2c.h"

#include <stdint.h>

// In this example we use I2CSetAddress to set an
// address that the ATMega328P will listen to in
// the I2C/TWI interface.
// We use I2CAttachSlaveRxCb to attach i2cOnReceive
// as a callback for the i2c receive event.
// When a receive is completed in the I2C the callback
// saves the received data into a separated buffer and
// prints its contents through serial

#define I2C_SLAVE_ADDR 0x55 // Own i2c address

#define I2C_RX_BUFFER_SIZE (I2C_BUFFER_SIZE + 1)

volatile uint8_t i2cRxBuffer[I2C_RX_BUFFER_SIZE];
volatile uint8_t i2cRxLength = 0;
volatile uint8_t printI2COutput = 0;

void i2cOnReceive(uint8_t *data, uint8_t dataLength);

int main()
{
    initTiming();
    initI2C();

    I2CSetAddress(I2C_SLAVE_ADDR);
    I2CAttachSlaveRxCb(i2cOnReceive);

    serialBegin(9600);

    serialPrintln("Slave initialized");

    while (1)
    {
        if (printI2COutput)
        {
            uint8_t len;
            char buffer[I2C_RX_BUFFER_SIZE];

            // Disable interrupts while we copy the buffer to a local variable.
            // This avoids it being suddenly overwritten
            CLI();
            len = i2cRxLength;
            for (uint8_t i = 0; i < len; i++)
            {
                buffer[i] = (char)i2cRxBuffer[i];
            }

            buffer[len] = '\0';

            printI2COutput = 0;
            SEI(); // Enable interrupts again

            serialPrint("I2C RX: ");
            serialPrintln(buffer);
        }
    }

    return 0;
}

void i2cOnReceive(uint8_t *data, uint8_t dataLength)
{
    uint8_t len = dataLength;

    if (len > I2C_RX_BUFFER_SIZE)
    {
        len = I2C_RX_BUFFER_SIZE;
    }

    for (uint8_t i = 0; i < len; i++)
    {
        i2cRxBuffer[i] = data[i];
    }

    i2cRxLength = len;
    printI2COutput = 1;
}
#include "timing.h"
#include "serial.h"
#include "interrupt.h"
#include "TwoWire.h"

#include <stdint.h>

// This example does the same as i2cSlaveReceiver example
// but it is implemented using the Wire class

#define I2C_SLAVE_ADDR 0x55 // Own i2c address

#define I2C_RX_BUFFER_SIZE (I2C_BUFFER_SIZE + 1)

volatile uint8_t i2cRxBuffer[I2C_RX_BUFFER_SIZE];
volatile uint8_t i2cRxLength = 0;
volatile uint8_t printI2COutput = 0;

void i2cOnReceive(uint8_t dataLength);

int main()
{
    initTiming();
    Wire.begin(I2C_SLAVE_ADDR);

    Wire.onReceive(i2cOnReceive);

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

void i2cOnReceive(uint8_t dataLength)
{
    uint8_t i = 0;

    while (Wire.available())
    {
        i2cRxBuffer[i] = Wire.read();
        i++;
    }

    i2cRxLength = dataLength;
    if (i2cRxLength > I2C_RX_BUFFER_SIZE)
    {
        i2cRxLength = I2C_RX_BUFFER_SIZE;
    }

    printI2COutput = 1;
}
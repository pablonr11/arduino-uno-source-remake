#include "timing.h"
#include "serial.h"
#include "interrupt.h"

#include "TwoWire.h"

#include <stdint.h>

// This example does the same as i2cSlaveTransmitter example
// but it is implemented using the Wire class

#define I2C_SLAVE_ADDR 0x55 // Own i2c address

#define I2C_RX_BUFFER_SIZE (I2C_BUFFER_SIZE + 1)

// Leds to power ON on slave
// 0xFE POWER ON ALL
// 0XFF POWER OFF ALL
static volatile uint8_t leds[] = {5, 6, 7, 0xFE, 0xFF};
static volatile uint8_t ledsIndex = 0;

void i2cOnRequest();

int main()
{
    initTiming();

    Wire.onRequest(i2cOnRequest);
    Wire.begin(I2C_SLAVE_ADDR);

    serialBegin(9600);

    serialPrintln("Slave initialized");

    while (1)
    {
    }

    return 0;
}

void i2cOnRequest()
{
    uint8_t led;

    if (ledsIndex >= sizeof(leds))
    {
        ledsIndex = 0;
    }

    led = leds[ledsIndex];
    ledsIndex++;

    Wire.write(led);
}

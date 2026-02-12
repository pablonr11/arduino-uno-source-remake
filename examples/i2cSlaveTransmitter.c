#include "timing.h"
#include "serial.h"
#include "interrupt.h"
#include "i2c.h"

#include <stdint.h>

// In this example we are sending through i2c the leds
// that the master should power on.
// In the leds array we have pins 5, 6 and 7. Also
// 0xFE is the code that indicates all pins should
// be powered, and 0xFF that indicates that all pins
// should be turned off.
// This example doesn't make any sense for a real
// case, but is an example that sends data over I2C
// and has a visual output in the master side.
// The master has the core implementation. What I did
// is connecting a button to pin 3 of the master
// as INPUT_PULLUP. And 3 leds on pins 5, 6 and 7.
// When the button is pressed is requests through I2C
// the next pin, and this (the slave) is responsible of
// handling which is the next one, and sends it to the
// master.

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
    initI2C();

    I2CSetAddress(I2C_SLAVE_ADDR);
    I2CAttachSlaveTxCb(i2cOnRequest);

    serialBegin(9600);

    serialPrintln("Slave initialized");

    while (1)
    {
    }

    return 0;
}

void i2cOnRequest()
{
    uint8_t led[1];

    if (ledsIndex >= sizeof(leds))
    {
        ledsIndex = 0;
    }

    led[0] = leds[ledsIndex];
    ledsIndex++;

    I2CSetSlaveTxData(led, 1);
}

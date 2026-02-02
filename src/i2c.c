#include "i2c.h"
#include "atmega328p.h"
#include "arduino_uno.h"
#include "digital.h"
#include "utils.h"

void initI2C(void)
{
    // Enable Pullup resistors for SDA and SCL
    digitalWrite(SDA, HIGH);
    digitalWrite(SCL, HIGH);

    // Set prescaler to 1
    TWSR &= ~(LSHB(TWPS1) | LSHB(TWPS0));

    // Set Bit Rate
    TWBR = (uint8_t)((F_CPU / I2C_FREQUENCY) - 16) / 2;

    // Enable acknowledge, enable i2c interface and enable i2c interrupt
    TWCR |= LSHB(TWEA) | LSHB(TWEN) | LSHB(TWIE);
}
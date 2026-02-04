#include "i2c.h"
#include "atmega328p.h"
#include "arduino_uno.h"
#include "digital.h"
#include "interrupt.h"
#include "utils.h"

static volatile i2c_state i2cState;
static volatile i2c_error i2cError;
static volatile uint8_t i2cSlaRW;
static volatile uint8_t i2cSendStop;
static volatile uint8_t i2cSentRepStart;

static uint8_t i2cBuffer[I2C_BUFFER_SIZE];
static volatile uint8_t i2cBufferIndex = 0;
static volatile uint8_t i2cBufferLength = 0;

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
    TWCR = LSHB(TWEA) | LSHB(TWEN) | LSHB(TWIE);

    i2cState = I2C_READY;
    i2cSendStop = 1;
    i2cSentRepStart = 0;
}

uint8_t I2CWrite(uint8_t address, uint8_t *data, uint8_t dataLength, uint8_t wait, uint8_t sendStop)
{
    // If data doesn't fit in the buffer just return
    if (dataLength > I2C_BUFFER_SIZE)
        return 1;

    // Wait until I2C is ready to start the transmission
    // We need to wait because if any other process
    // is taking place we shouldn't interact with the line.
    // For example while in slave mode.
    while (i2cState != I2C_READY)
        ;

    // Set the new state
    i2cState = I2C_MASTER_TX;
    // Restart error to no error
    i2cError = I2C_NO_ERROR;
    i2cSendStop = sendStop;

    // Reset index and length values for the buffer
    i2cBufferIndex = 0;
    i2cBufferLength = dataLength;

    // Save the data in the buffer to be processed by
    // the interrupt
    for (uint8_t i = 0; i < dataLength; i++)
    {
        i2cBuffer[i] = data[i];
    }

    // Store slave address + write
    i2cSlaRW = I2C_WRITE; // Sets all bits to 0. Also we need LSB to be 0 to perform a write
    i2cSlaRW |= address << 1;

    if (i2cSentRepStart)
    {
        i2cSentRepStart = 0;

        // If we are already in a repeated START we have
        // to load SLA+R in TWDR
        do
        {
            TWDR = i2cSlaRW;
        } while (TWCR & LSHB(TWWC)); // If we tried to write but we couldn't try to write again

        // Enables interrupts again
        // Basicaly restores the original configuration
        I2CContinue(1);
    }
    else
    {
        // Send START
        // Setting TWSTA transmits a START
        TWCR = LSHB(TWINT) | LSHB(TWEA) | LSHB(TWSTA) | LSHB(TWEN) | LSHB(TWIE);
    }

    // Wait until all data has been sent
    while (wait && i2cState == I2C_MASTER_TX)
        ;

    if (i2cState == I2C_NO_ERROR)
        return 0;
    else if (i2cState == I2C_MT_SLAW_NACK_ERROR)
        return 2;
    else if (i2cState == I2C_MT_DATA_NACK_ERROR)
        return 3;
    else if (i2cState == I2C_MT_ARBITRATION_LOST_ERROR)
        return 4;

    return 5; // Shouldn't reach here?
}

void I2CContinue(uint8_t ack)
{
    if (ack)
    {
        TWCR = LSHB(TWINT) | LSHB(TWEA) | LSHB(TWEN) | LSHB(TWIE);
    }
    else
    {
        TWCR = LSHB(TWINT) | LSHB(TWEN) | LSHB(TWIE);
    }
}

void I2CStop(void)
{
    // Send STOP condition
    TWCR = LSHB(TWINT) | LSHB(TWEA) | LSHB(TWSTO) | LSHB(TWEN) | LSHB(TWIE);

    // Wait until the stop has been sent
    while (TWCR & LSHB(TWSTO))
        ;

    i2cState = I2C_READY;
}

void I2CRelease(void)
{
    // Release the bus
    TWCR = LSHB(TWINT) | LSHB(TWEA) | LSHB(TWEN) | LSHB(TWIE);

    // Set state to ready
    i2cState = I2C_READY;
}

ISR(TWI_vect)
{
    switch (I2C_STATUS)
    {
    case I2C_START:
    case I2C_REPEATED_START:
        TWDR = i2cSlaRW; // Write slave address + R/W bit
        I2CContinue(1);  // Clear TWINT flag
        break;
    // Master Transmitter Mode Statuses
    case I2C_MT_SLAW_ACK:
    case I2C_MT_DATA_ACK:
        // Check if there are more bytes to send
        if (i2cBufferIndex < i2cBufferLength)
        {
            TWDR = i2cBuffer[i2cBufferIndex]; // Set data to send in the output buffer
            I2CContinue(1);                   // Clear TWINT flag
            i2cBufferIndex++;
            break;
        }
        else
        {
            if (i2cSendStop)
            {
                I2CStop();
            }
            else
            {
                // If an stop is not sent here we can assume the bus
                // is going to be used again so we can send another
                // start here. Since we don't have the new data we
                // have to disable interrupts until the next step is
                // taken. Here we save a flag indicating we are already
                // in a repeated start, so in the next iteration
                // outside the ISR we should write SLARW
                TWCR = LSHB(TWINT) | LSHB(TWEA) | LSHB(TWSTA) | LSHB(TWEN);
                i2cSentRepStart = 1;
                i2cState = I2C_READY;
            }
        }
        break;
    case I2C_MT_SLAW_NACK:
        i2cError = I2C_MT_SLAW_NACK_ERROR;
        I2CStop();
        break;
    case I2C_MT_DATA_NACK:
        i2cError = I2C_MT_DATA_NACK_ERROR;
        I2CStop();
        break;
    case I2C_MT_ARBITRATION_LOST:
        i2cError = I2C_MT_ARBITRATION_LOST_ERROR;
        I2CRelease();
        break;
    }
}
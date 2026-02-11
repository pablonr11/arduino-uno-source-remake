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

static uint8_t i2cMasterBuffer[I2C_BUFFER_SIZE];
static volatile uint8_t i2cMasterBufferIndex = 0;
static volatile uint8_t i2cMasterBufferLength = 0;

static uint8_t i2cSlaveTxBuffer[I2C_BUFFER_SIZE];
static volatile uint8_t i2cSlaveTxBufferIndex = 0;
static volatile uint8_t i2cSlaveTxBufferLength = 0;

static uint8_t i2cSlaveRxBuffer[I2C_BUFFER_SIZE];
static volatile uint8_t i2cSlaveRxBufferIndex = 0;

static i2cOnSlaveRx i2cOnSlaveRxCb;
static i2cOnSlaveTx i2cOnSlaveTxCb;

void initI2C(void)
{
    // Enable Pullup resistors for SDA and SCL
    digitalWrite(SDA, HIGH);
    digitalWrite(SCL, HIGH);

    // Set prescaler to 1
    TWSR &= ~(LSHB(TWPS1) | LSHB(TWPS0));

    // Set Bit Rate
    TWBR = (uint8_t)(((F_CPU / I2C_FREQUENCY) - 16) / 2);

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
    i2cMasterBufferIndex = 0;
    i2cMasterBufferLength = dataLength;

    // Save the data in the buffer to be processed by
    // the interrupt
    for (uint8_t i = 0; i < dataLength; i++)
    {
        i2cMasterBuffer[i] = data[i];
    }

    // Store slave address + write
    i2cSlaRW = I2C_WRITE; // Sets all bits to 0. Also we need LSB to be 0 to perform a write
    i2cSlaRW |= address << 1;

    if (i2cSentRepStart)
    {
        i2cSentRepStart = 0;

        // If we are already in a repeated START we have
        // to load SLA+W in TWDR
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

    if (i2cError == I2C_NO_ERROR)
        return 0;
    else if (i2cError == I2C_MT_SLAW_NACK_ERROR)
        return 2;
    else if (i2cError == I2C_MT_DATA_NACK_ERROR)
        return 3;
    else if (i2cError == I2C_MT_ARBITRATION_LOST_ERROR)
        return 4;

    return 5; // Shouldn't reach here?
}

uint8_t I2CRead(uint8_t address, uint8_t *data, uint8_t dataLength, uint8_t sendStop)
{
    // If data doesn't fit in the buffer just return
    if (dataLength > I2C_BUFFER_SIZE)
        return 0;

    // Wait until I2C is ready to start the transmission
    // We need to wait because if any other process
    // is taking place we shouldn't interact with the line.
    // For example while in slave mode.
    while (i2cState != I2C_READY)
        ;

    i2cState = I2C_MASTER_RX; // Set the new state
    i2cSendStop = sendStop;   // Save if we are going to send STOP at the end

    // Reset index and length values for the buffer
    i2cMasterBufferIndex = 0;
    // We need to substract 1 from length.
    // The reason for that is because after sending SLAR+ACK
    // we will get already a byte.
    // For example: If we want to read 1 byte
    // After sending SLAR and receiving ACK the slave will start
    // sending the byte inmediatly. Once we obtained the byte we don't
    // want to send an ACK because this will fetch another byte.
    // So, index (0) < dataLength - 1 (0) is false. In I2C_MR_SLAR_ACK
    // case we are configuring the NACK for that case.
    i2cMasterBufferLength = dataLength - 1;

    // Store slave address + read
    i2cSlaRW = I2C_READ; // Sets all bits to 0 and 1 in the LSB. LSB has to be 1 to send READ
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

    // Wait until reading has finished
    while (i2cState == I2C_MASTER_RX)
        ;

    // Now i2cMasterBufferIndex should equal the number of bytes
    // received. i2cMasterBufferIndex is always increased after
    // reading from TWDR and writing to the buffer

    // Copy the read data from the buffer to data
    for (uint8_t i = 0; i < i2cMasterBufferIndex; i++)
    {
        data[i] = i2cMasterBuffer[i];
    }

    return i2cMasterBufferIndex;
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

void I2CSetAddress(uint8_t address)
{
    // Bit 0 in TWAR is to enable/listen for general calls (0x00)
    // By default we are disabling it
    TWAR = (address << 1);
}

void I2CAttachSlaveRxCb(i2cOnSlaveRx cb)
{
    i2cOnSlaveRxCb = cb;
}

void I2CAttachSlaveTxCb(i2cOnSlaveTx cb)
{
    i2cOnSlaveTxCb = cb;
}

uint8_t I2CSetSlaveTxData(uint8_t *data, uint8_t dataLength)
{
    // If the data is to big return error
    if (dataLength + i2cSlaveTxBufferLength > I2C_BUFFER_SIZE)
    {
        return 0;
    }

    // Append data to the buffer
    for (uint8_t i = 0; i < dataLength; i++)
    {
        i2cSlaveTxBuffer[i + i2cSlaveTxBufferLength] = data[i];
    }
    i2cSlaveTxBufferLength += dataLength;

    return 1;
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
        if (i2cMasterBufferIndex < i2cMasterBufferLength)
        {
            TWDR = i2cMasterBuffer[i2cMasterBufferIndex]; // Set data to send in the output buffer
            I2CContinue(1);                               // Clear TWINT flag
            i2cMasterBufferIndex++;
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
    // Master Receiver Mode Statuses
    case I2C_MR_DATA_ACK:
        i2cMasterBuffer[i2cMasterBufferIndex] = TWDR;
        i2cMasterBufferIndex++;
        // ! This continues in the next case
    case I2C_MR_SLAR_ACK:
        // If there are more bytes to read send ACK. Otherwise send NACK
        I2CContinue((uint8_t)(i2cMasterBufferIndex < i2cMasterBufferLength));
        break;
    case I2C_MR_DATA_NACK:
        i2cMasterBuffer[i2cMasterBufferIndex] = TWDR; // Last received byte
        i2cMasterBufferIndex++;
        // Now we should send an stop or a repeated start
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
        break;
    case I2C_MR_SLAR_NACK:
        I2CStop();
        break;

    case I2C_ARBITRATION_LOST:
        i2cError = I2C_MT_ARBITRATION_LOST_ERROR;
        I2CRelease();
        break;
    // Slave Receiver Mode Statuses
    case I2C_SR_SLAW_ACK:
    case I2C_SR_MARBLOST_SLAW_ACK:
    case I2C_SR_GENERAL_CALL_ACK:
    case I2C_SR_MARBLOST_GENERAL_CALL_ACK:
        i2cState = I2C_SLAVE_RX;   // Set i2c state to avoid master modes
        i2cSlaveRxBufferIndex = 0; // Set index to override rx buffer
        I2CContinue(1);            // Send ACK when we receive the first byte
        break;
    case I2C_SR_SLAW_DATA_ACK:
    case I2C_SR_GENERAL_CALL_DATA_ACK:
        if (i2cSlaveRxBufferIndex < I2C_BUFFER_SIZE)
        {
            i2cSlaveRxBuffer[i2cSlaveRxBufferIndex] = TWDR; // Save data to buffer
            i2cSlaveRxBufferIndex++;                        // Increase buffer index
            I2CContinue(1);                                 // Send ACK
        }
        else
        {
            I2CContinue(0); // Senc NACK
        }
        break;
    case I2C_SR_SLAW_DATA_NACK:
    case I2C_SR_GENERAL_CALL_DATA_NACK:
        // Here we received one more byte from the master
        // but we can't store it in the buffer because is full.
        // After sending the NACK the master should send a STOP
        // so we disable ACKs here and then enable them again
        // in I2C_SR_STO_STA case.
        I2CContinue(0);
        break;
    case I2C_SR_STO_STA:
        // Enable ACKs again and set i2c state to ready
        I2CRelease();

        // Insert null character if there's more space in the buffer
        if (i2cSlaveRxBufferIndex < I2C_BUFFER_SIZE)
        {
            i2cSlaveRxBuffer[i2cSlaveRxBufferIndex] = '\0';
        }

        i2cOnSlaveRxCb(i2cSlaveRxBuffer, i2cSlaveRxBufferIndex);

        break;
    // Slave Transmitter Mode Statuses
    case I2C_ST_SLAR_ACK:
    case I2C_ST_MARBLOST_SLAR_ACK:
        i2cState = I2C_SLAVE_TX; // Set i2c state to avoid master modes
        // Restart TX buffer index and length before calling
        // onSlaveTx callback.
        // The callback could call I2CSetSlaveTxData multiple times.
        i2cSlaveTxBufferIndex = 0;
        i2cSlaveTxBufferLength = 0;

        // Call the callback. This should fill the buffer
        // The user has to call I2CSetSlaveTxData to fill
        // the buffer
        i2cOnSlaveTxCb();

        // We need to send at least one byte.
        // If the buffer is still empty send a 0x00 and NACK
        if (i2cSlaveTxBufferLength == 0)
        {
            i2cSlaveTxBuffer[0] = 0;
            i2cSlaveTxBufferLength = 1;
        }
        // ! This continues in the next case.
        // ! The next case sends the data
    case I2C_ST_DATA_ACK:
        // Load the byte in TWDR
        TWDR = i2cSlaveTxBuffer[i2cSlaveTxBufferIndex];
        i2cSlaveTxBufferIndex++;

        // Here the TWEA bit in TWCR indicates if the
        // last byte has been sent.
        // If there are more bytes to send set TWEA
        // Otherwise clear it.
        if (i2cSlaveTxBufferIndex < i2cSlaveTxBufferLength)
        {
            I2CContinue(1); // There's more data
        }
        else
        {
            I2CContinue(0); // No more data to send
        }
        break;
    case I2C_ST_DATA_NACK:
    case I2C_ST_LAST_BYTE_ACK:
        // If we receive a NACK or don't have more data to send

        // If we reached here because we don't have more byte
        // we should enable ACKs again for the future
        I2CContinue(1);
        i2cState = I2C_READY;
        break;
    }
}
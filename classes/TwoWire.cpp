#include "i2c.h"
#include "TwoWire.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

uint8_t TwoWire::rxBuffer[I2C_BUFFER_SIZE];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txBuffer[I2C_BUFFER_SIZE];
uint8_t TwoWire::transmitting = 0;
uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

void (*TwoWire::userOnReceive)(uint8_t);
void (*TwoWire::userOnRequest)(void);

TwoWire::TwoWire()
{
}

void TwoWire::begin()
{
    initI2C();
    I2CAttachSlaveRxCb(onReceiveCb);
    I2CAttachSlaveTxCb(onRequestCb);
}

void TwoWire::begin(uint8_t address)
{
    begin();
    I2CSetAddress(address);
}

// Indicates that the user is going to do a transmission.
// Saves the address to transmit to, restores index and length
// and sets the transmitting flag
void TwoWire::beginTransmission(uint8_t address)
{
    transmitting = 1;

    txBufferIndex = 0;
    txBufferLength = 0;
    txAddress = address;
}

size_t TwoWire::write(uint8_t data)
{
    if (transmitting)
    {
        // If we are master transmitter save the byte to send it
        // in endTransmission
        // If the buffer is full just return 0
        if (txBufferIndex >= I2C_BUFFER_SIZE)
            return 0;

        txBuffer[txBufferIndex] = data;
        txBufferIndex++;
        txBufferLength++;
    }
    else
    {
        // If we are not master transmitter we should be slave transmitter
        // just send the data to the bus
        if (I2CSetSlaveTxData(&data, 1) != 1)
            return 0;
    }
    return 1;
}

size_t TwoWire::write(char *string)
{
    size_t stringLength;

    stringLength = strlen(string);
    return (write((uint8_t *)string, stringLength));
}

size_t TwoWire::write(const uint8_t *data, size_t dataLength)
{
    // Counter to track how many bytes are sent
    size_t transmitted = 0;

    for (size_t i = 0; i < dataLength; i++)
    {
        size_t written = write(data[i]);

        // If the byte is sent correctly increment transmitted
        // Otherwise just exit the loop and return transmitted
        if (written)
            transmitted++;
        else
            break;
    }

    return transmitted;
}

// Sends the bytes queued by write() and ends the transmission.
// beginTransmission() needs to be called before.
// If stop == 1 an stop will be send at the end
// If stop == 0 a repeated start will be send at the end
// Returns an error code:
// 0 -> success/no errror
// 1 -> data length is to long for the buffer
// 2 -> NACK received after sending SLAW
// 3 -> NACK received after sending data
// 4 -> other error
uint8_t TwoWire::endTransmission(uint8_t stop)
{
    uint8_t status;

    if (!transmitting)
        return 4;

    status = I2CWrite(txAddress, txBuffer, txBufferLength, 1, stop);

    // Reset buffer variables
    txBufferIndex = 0;
    txBufferLength = 0;
    transmitting = 0;

    return status;
}

uint8_t TwoWire::endTransmission()
{
    return endTransmission(1);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t stop)
{
    uint8_t read = 0;

    if (transmitting)
        return 0;

    read = I2CRead(address, rxBuffer, quantity, stop);
    rxBufferIndex = 0;
    rxBufferLength = read;

    return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom(address, quantity, 1);
}

uint8_t TwoWire::available()
{
    return (rxBufferLength - rxBufferIndex);
}

// Here the returned value should be treated as a single byte
// This means the 8 MSB of the return value should be ignored.
// We need to return a int (2 bytes) because we return -1
// when there are no more bytes to read.
int TwoWire::read()
{
    uint8_t value = -1;

    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
        rxBufferIndex++;
    }

    return value;
}

void TwoWire::onReceive(void (*cb)(uint8_t))
{
    userOnReceive = cb;
}

void TwoWire::onRequest(void (*cb)(void))
{
    userOnRequest = cb;
}

// Function called when receiving something as a slave
void TwoWire::onReceiveCb(uint8_t *data, uint8_t length)
{
    if (!userOnReceive)
        return;

    // Save the received data to the local buffer
    // This allows the use of available and read
    // to obtain the received data.
    for (uint8_t i = 0; i < length; i++)
    {
        rxBuffer[i] = data[i];
    }

    rxBufferIndex = 0;
    rxBufferLength = length;

    userOnReceive(length);
}

// Function called when a request is received as a slave
void TwoWire::onRequestCb(void)
{
    if (!userOnRequest)
        return;

    txBufferIndex = 0;
    txBufferLength = 0;

    userOnRequest();
}

TwoWire Wire = TwoWire();
#ifndef _TWOWIRE_H
#define _TWOWIRE_H

#include "i2c.h"

#include <stddef.h>
#include <stdint.h>

class TwoWire
{
public:
    TwoWire();

    void begin(void);
    void begin(uint8_t);
    void beginTransmission(uint8_t);
    size_t write(uint8_t);                 // Sends specified byte
    size_t write(char *);                  // Sends a string. The string must be null terminated
    size_t write(const uint8_t *, size_t); // Sends an array of bytes.
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t available(void);
    int read(void);
    void onReceive(void (*)(uint8_t));
    void onRequest(void (*)(void));

private:
    // Local rx buffer variables
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    // local tx buffer variables
    static uint8_t transmitting;
    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static void onReceiveCb(uint8_t *, uint8_t);
    static void onRequestCb(void);
    static void (*userOnReceive)(uint8_t);
    static void (*userOnRequest)(void);
};

extern TwoWire Wire;

#endif
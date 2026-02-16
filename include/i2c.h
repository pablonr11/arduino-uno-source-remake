#ifndef _I2C_H
#define _I2C_H

#include "atmega328p.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

// I2C_FREQUENCY = F_CPU / (16 + 2 * TWBR * PrescalerValue)
// This value is used to calculate TWBR
// We need to obtain a formula to calculate TWBR.
// The whole process to separate TWBR in the previous formula is the following:
// 1. I2C_FREQUENCY = F_CPU / (16 + 2 * TWBR * PrescalerValue)
// 2. We use PrescalerValue 1 so we can remove it
//      I2C_FREQUENCY = F_CPU / (16 + 2 * TWBR)
// 3. I2C_FREQUENCY * (16 + 2 * TWBR) = F_CPU
// 4. 16 + 2 * TWBR = F_CPU / I2C_FREQUENCY
// 5. 2 * TWBR = (F_CPU / I2C_FREQUENCY) - 16
// 6. TWBR = ((F_CPU / I2C_FREQUENCY) - 16) / 2
#define I2C_FREQUENCY 100000

#define I2C_STATUS (TWSR & I2C_STATUS_MASK)
#define I2C_BUFFER_SIZE 64
#define I2C_WRITE 0
#define I2C_READ 1

// I2C possible status values in TWSR
#define I2C_START 0x08
#define I2C_REPEATED_START 0x10
// Master transmitter
#define I2C_MT_SLAW_ACK 0x18
#define I2C_MT_SLAW_NACK 0x20
#define I2C_MT_DATA_ACK 0x28
#define I2C_MT_DATA_NACK 0x30
#define I2C_ARBITRATION_LOST 0x38
// Master receiver
#define I2C_MR_SLAR_ACK 0x40
#define I2C_MR_SLAR_NACK 0x48
#define I2C_MR_DATA_ACK 0x50
#define I2C_MR_DATA_NACK 0x58
// Slave receiver
#define I2C_SR_SLAW_ACK 0x60
#define I2C_SR_MARBLOST_SLAW_ACK 0x68 // Arbitration lost as master, received own SLA+W and sent ACK
#define I2C_SR_GENERAL_CALL_ACK 0x70
#define I2C_SR_MARBLOST_GENERAL_CALL_ACK 0x78 // Arbitration lost as master, general call received and sent ACK
#define I2C_SR_SLAW_DATA_ACK 0x80
#define I2C_SR_SLAW_DATA_NACK 0x88
#define I2C_SR_GENERAL_CALL_DATA_ACK 0x90
#define I2C_SR_GENERAL_CALL_DATA_NACK 0x98
#define I2C_SR_STO_STA 0xA0
// Slave transmitter
#define I2C_ST_SLAR_ACK 0xA8
#define I2C_ST_MARBLOST_SLAR_ACK 0xB0
#define I2C_ST_DATA_ACK 0xB8
#define I2C_ST_DATA_NACK 0xC0
#define I2C_ST_LAST_BYTE_ACK 0xC8

    // Callback type for slave recieve
    typedef void (*i2cOnSlaveRx)(uint8_t *, uint8_t);
    // Callback type for slave transmitter
    typedef void (*i2cOnSlaveTx)();
    typedef enum
    {
        I2C_READY = 0,
        I2C_MASTER_TX,
        I2C_MASTER_RX,
        I2C_SLAVE_RX,
        I2C_SLAVE_TX,
    } i2c_state;

    typedef enum
    {
        I2C_MT_SLAW_NACK_ERROR = 0,
        I2C_MT_DATA_NACK_ERROR,
        I2C_MT_ARBITRATION_LOST_ERROR,
        I2C_NO_ERROR = 0xFF
    } i2c_error;

    /**
     * @brief Initializes the I2C/TWI interface
     * When calling this function the pins SDA and SCL
     * will be used for the TWI interface. The pullup
     * resistors in the two pins will be enabled.
     * The prescaler defaults to 1 and the bit rate will
     * be set using I2C_FREQUENCY which defaults to 100000.
     * Finally the module is enabled.
     */
    void initI2C(void);

    /**
     * @brief Used to send data as master transmitter
     * @param address The address of the device we want to
     * write to. Addresses are 7 bits. The MSB will be ignored.
     * @param data Array with the data we want to write to the device.
     * @param dataLength Number of bytes in data.
     * @param wait Indicates if we need to wait until all the data has been sent
     * @param stop If is true a stop condition will be sent when there's no more data
     * to send. If false no stop condition will be sent.
     * @return 0 if no error ocurred during transmission.
     * 1 If data length is to long for the buffer
     * 2 NACK received after sending SLAW
     * 3 NACK received after sending data
     * 4 arbitration lost
     * 5 other error
     */
    uint8_t I2CWrite(uint8_t address, uint8_t *data, uint8_t dataLength, uint8_t wait, uint8_t sendStop);

    /**
     * @brief Used to read data as master receiver
     * @param address The addres of the device we want to
     * read from. Addresses are 7 bits. The MSB will be ignored
     * @param data Array were the response will be outputed
     * @param dataLength Number of bytes to read
     * @param stop If is true a stop condition will be sent when there's no more data
     * to read. If false no stop condition will be sent.
     * @return Number of bytes read.
     */
    uint8_t I2CRead(uint8_t address, uint8_t *data, uint8_t dataLength, uint8_t sendStop);

    /**
     * @brief This is mainly used to clear the TWINT flag.
     * Additionaly it sets TWEN and TWIE to 1.
     * @param ack if is 1 TWEA will be set to 1. Otherwise it will be set to 0
     */
    void I2CContinue(uint8_t ack);

    /**
     * @brief Send a stop condition.
     * The TWSTO flag is not cleared until the stop condition
     * has been completed. The function waits until this flag
     * is cleared by HW.
     */
    void I2CStop(void);

    /**
     * @brief Releases the bus control.
     * This function should be used when something went
     * wront with a transmission and a stop can't be sent.
     */
    void I2CRelease(void);

    /**
     * @brief Sets the address for slave mode
     * @param address The address to listen to.
     * The address is 7 bits long, so the MSB will be ignored.
     */
    void I2CSetAddress(uint8_t address);

    /**
     * @brief Used to attach a function callback for the slave
     * receive. The attached function will be called once
     * the transmission in finished. (When the master sends a
     * STOP or a REPEATED START)
     * @param cb The callback function
     */
    void I2CAttachSlaveRxCb(i2cOnSlaveRx cb);

    /**
     * @brief Used to attach a function callback for the slave
     * transmit. The attached function will be called before the
     * transmission is started, after receiving SLAR and sending
     * the ACK.
     * @param cb The callback function
     */
    void I2CAttachSlaveTxCb(i2cOnSlaveTx cb);

    /**
     * @brief Calling this function sets the data that will
     * be sent when the slave transmitter mode is entered.
     * This function should be called in the i2cOnSlaveTx
     * callback.
     * @param data A pointer to the data to be sent
     * @param dataLength Length of the data to be sent. Has to be lower than I2C_BUFFER_SIZE.
     * @return Error code:
     * 0 -> dataLength to big
     * 1 -> No error
     */
    uint8_t I2CSetSlaveTxData(uint8_t *data, uint8_t dataLength);

#ifdef __cplusplus
}
#endif

#endif
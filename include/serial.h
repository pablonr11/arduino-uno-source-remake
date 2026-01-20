#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>

#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 64

/**
 * ATMega328P uses the pins RXD and TXD (PD0 and PD1 aka. D0 and D1
 * in the Arduino UNO R3) as the receive and transmit pins for USART.
 *
 * Arduino UNO R3 has an additional non programable chip ATMega16U2
 * that handles the Serial-to-USB conversion. This additional chip
 * allows us to see and send data through USART from the ATMega328P
 * in a serial monitor in a computer if we have Arduino UNO
 * connected to one via USB.
 */

/**
 * @brief Configures the serial. Calling this function is needed
 * before being able to use prints
 * @param baudrate The target baudrate. The standard for Arduino
 * UNO is 9600.
 */
void serialBegin(uint32_t baudrate);

/**
 * @brief Sends a list of characters through the serial.
 * @param str A pointer to the data sent through the terminal.
 * Should contain a null terminator.
 */
void serialPrint(const char *str);

/**
 * @brief Sends a list of character through the serial and
 * adds a line break at the end.
 * @param str A pointer to the data sent through the terminal.
 * Should contain a null terminator.
 */
void serialPrintln(const char *str);

/**
 * @brief Checks if there are available bytes to read in the rx buffer.
 * @return The amount of bytes to read from the rx buffer
 */
uint8_t serialAvailable(void);

/**
 * @brief Gets the last unread character in the rx buffer.
 * @return The obtained byte from the RX buffer.
 */
int serialRead(void);

/**
 * @brief Closes the serial communication. Doing so allows the RX and TX
 * pins to be used as GPIOs.
 */
void serialEnd(void);

/**
 * @brief Waits until the previous transmition is completed.
 */
void serialFlush(void);

#endif
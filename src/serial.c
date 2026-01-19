#include "serial.h"
#include "atmega328p.h"
#include "interrupt.h"

#include <stdint.h>

void serialBegin(uint32_t baudrate)
{
    // Calculate the baud rate register value first
    uint32_t ubrr = F_CPU / (8 * baudrate) - 1;
    // Enable double transmission speed
    UCSR0A |= LSHB(U2X0);

    // If ubrr doesn't fit in 12 bit we have to disable double transmission speed
    if (ubrr > 4095)
    {
        ubrr = F_CPU / (16 * baudrate) - 1;
        UCSR0A &= ~LSHB(U2X0);
    }

    // Technicaly we don't need to change UCSR0C because it's default value already
    // sets asynchronous mode, parity check disabled, 1 stop bit and
    // character size to 8 bit.
    // UCSZ01 and UCSZ00 are set to 1 by default, but for security will set them manually
    UCSR0C |= LSHB(UCSZ01) | LSHB(UCSZ00);

    // Configure the baudrate
    UBRR0H = ubrr >> 8; // Shifting right 8 times truncates the lower part
    UBRR0L = ubrr;      // The higher part get truncated because UBRR0L is an 8 bit register

    // Once the configuration is done we can enable receiver, transmitter and
    // rx complete interrupt
    UCSR0B |= LSHB(RXEN0) | LSHB(TXEN0);
}

static void serialWrite(const char chr)
{
    // Wait until the transmit buffer is ready to receive new data
    while (!(UCSR0A & LSHB(UDRE0)))
        ;

    UDR0 = chr; // Write the character to be sent.
}

void serialPrint(const char *str)
{
    uint16_t i = 0;
    do
    {
        serialWrite(str[i]);
        i++;
    } while (str[i] != '\0');
}

void serialPrintln(const char *str)
{
    serialPrint(str);
    serialPrint("\r\n");
}

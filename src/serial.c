#include "serial.h"
#include "atmega328p.h"
#include "interrupt.h"

#include <stdint.h>

// RX ring buffer. Stores the received bytes through serial.
static volatile char rxBuffer[RX_BUFFER_SIZE] = {0};
static volatile uint8_t rxHead = 0; // Last write position
static volatile uint8_t rxTail = 0; // Last read position

// TX ring buffer. Stores the bytes to send through serial.
// Using a software buffer for tx and using Data Register Empty
// Interrupt we can make transmitions of large data less blocking.
static volatile char txBuffer[TX_BUFFER_SIZE] = {0};
static volatile uint8_t txHead = 0;
static volatile uint8_t txTail = 0;

ISR(USART_RX_vect)
{
    uint8_t next = (rxHead + 1) % RX_BUFFER_SIZE;
    uint8_t data = UDR0;

    // If we are gonna write where the tail is this means
    // we are overwritting data, so we just ignore the received byte
    if (next != rxTail)
    {
        rxBuffer[rxHead] = data;
        rxHead = next;
    }
}

ISR(USART_UDRE_vect)
{
    if (txTail == txHead)
    {
        UCSR0B &= ~LSHB(UDRIE0); // Disable interrupt
        return;
    }

    UDR0 = txBuffer[txTail];
    txTail = (txTail + 1) % TX_BUFFER_SIZE;
}

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
    UCSR0B |= LSHB(RXEN0) | LSHB(TXEN0) | LSHB(RXCIE0);
    // Technically this shouldn't be necessary but we ensure
    // Data Empty Registry Interrupt is disabled.
    // We need to be carefull with this interrupt because
    // enabling it will generate interrupts while the
    // data register is empty.
    UCSR0B &= ~LSHB(UDRIE0);
}

static void serialWrite(const char chr)
{
    uint8_t next = (txHead + 1) % TX_BUFFER_SIZE;

    // Only block if tx buffer is full. In this case we need
    // to wait until the UDRE interrupt sends some data
    while (next == txTail)
        ;

    txBuffer[txHead] = chr;
    txHead = next;

    // Enable UDRE interrupt
    UCSR0B |= LSHB(UDRIE0);
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

uint8_t serialAvailable(void)
{
    return (uint8_t)(RX_BUFFER_SIZE + rxHead - rxTail) % RX_BUFFER_SIZE;
}

int serialRead(void)
{
    if (rxHead == rxTail)
        return -1;

    char data = rxBuffer[rxTail];
    rxTail = (rxTail + 1) % RX_BUFFER_SIZE;

    return data;
}

void serialEnd(void)
{
    UCSR0B &= ~(LSHB(RXEN0) | LSHB(TXEN0) | LSHB(RXCIE0));

    rxHead = 0;
    rxTail = 0;
}

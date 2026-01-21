#include "analog.h"
#include "utils.h"
#include "atmega328p.h"

#include <stdint.h>

void analogReference(uint8_t ref)
{
    ADMUX = (ADMUX & ~(LSHB(REFS1) | LSHB(REFS0))) | ((ref & 0x03) << REFS0);
}

void initAnalog(void)
{
    // Set voltage reference to use AVcc.
    // The Arduino UNO integrates a capacitor in AREF pin.
    analogReference(DEFAULT_ANALOG_REFERENCE);

    // We use right adjust result
    // For left adjust we would have to run this line
    // ADMUX |= LSHB(ADLAR);

#if F_CPU == 16000000
    // Set prescaler to 128 divisor and enabling ADC.
    // Setting the prescaler to 128 allows for maximum resolution
    // when using 10 bits.
    // This prescaler value is the default configuration for 16MHz.
    // "an input clock frequency between 50kHz and 200kHz to get
    // maximum resolution" (16000000Hz / 128 = 125000Hz)
    ADCSRA = LSHB(ADEN) | LSHB(ADPS2) | LSHB(ADPS1) | LSHB(ADPS0);
#else
#warning "initAnalog not implemented for F_CPU value"
#endif

    // The first conversion is always slower and less precise.
    // We make a dummy conversion to stabilize ADC
    ADCSRA |= LSHB(ADSC);
    while (ADCSRA & LSHB(ADSC))
        ;
}

int16_t analogRead(analog_pin pin)
{
    uint8_t low, high;
    // We set the pin we want to read from
    // (ADMUX & 0xF0) Mantain REFS and ADLAR
    // (pin & 0x0F) All MUX bits
    ADMUX = (ADMUX & 0xF0) | (pin & 0x0F);

    ADCSRA |= LSHB(ADSC); // Start conversion

    // Wait until the conversion finishes
    while (ADCSRA & LSHB(ADSC))
        ;

    // We need to read first ADCL
    low = ADCL;
    high = ADCH;

    return (high << 8) | low;
}
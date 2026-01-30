#include "analog.h"
#include "utils.h"
#include "digital.h"
#include "arduino_uno.h"
#include "atmega328p.h"

#include <stdint.h>

static inline void initADC(void)
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

static inline void initPWM(void)
{
    // Timer/Counter0 is already configured in timer0ConfigPwm()
    // which is called from initTiming

    // Timer/Counter1 Configuration
    TCCR1A = LSHB(WGM10);                  // Set phase correct 8bit mode
    TCCR1B = (TIMER_PRESCALER_64 << CS10); // Set prescaler to 64 and enable timer

    // Timer/Counter2 Configuration
    TCCR2A = LSHB(WGM20); // Set phase correct pwm mode
    TCCR2B = LSHB(CS22);  // Set prescaler to 64 and enable timer
}

void analogReference(uint8_t ref)
{
    ADMUX = (ADMUX & ~(LSHB(REFS1) | LSHB(REFS0))) | ((ref & 0x03) << REFS0);
}

void initAnalog(void)
{
    initPWM();
    initADC();
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

void analogWrite(uint8_t pin, uint8_t value)
{
    uint8_t timer = pinToTimerConversor[pin];

    if (timer == NOT_TIMER)
        return; // If the pin doesn't support PWM just return

    // If the pin supports PWM we need to ensure its DDR is
    // output
    pinMode(pin, OUTPUT);

    // If the value is on the edges of the timer this means
    // the value for the pulse will always be the same.
    // We can just write it's value using digitalWrite
    if (value == 0)
    {
        digitalWrite(pin, LOW);
        return;
    }
    else if (value == 255)
    {
        digitalWrite(pin, HIGH);
        return;
    }

    switch (timer)
    {
    case T0A:
        TCCR0A |= LSHB(COM0A1); // Connect OC0A (pin 6) to PWM
        OCR0A = value;
        break;
    case T0B:
        TCCR0A |= LSHB(COM0B1); // Connect OC0B (pin 5) to PWM
        OCR0B = value;
        break;
    case T1A:
        TCCR1A |= LSHB(COM1A1); // Connect OC1A (pin 9) to PWM
        OCR1A = value;
        break;
    case T1B:
        TCCR1A |= LSHB(COM1B1); // Connect OC1B (pin 10) to PWM
        OCR1B = value;
        break;
    case T2A:
        TCCR2A |= LSHB(COM2A1); // Connect OC2A (pin 11) to PWM
        OCR2A = value;
        break;
    case T2B:
        TCCR2A |= LSHB(COM2B1); // Connect OC2B (pin 3) to PWM
        OCR2B = value;
        break;
    }
}

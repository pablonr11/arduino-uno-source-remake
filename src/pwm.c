#include "atmega328p.h"
#include "pwm.h"

void initPWM(void)
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

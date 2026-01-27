#include "timing.h"
#include "analog.h"
#include "digital.h"
#include "arduino_uno.h"
#include "pwm.h"

int main()
{
    uint16_t analogValue = 0;

    initTiming();
    initPWM();
    initAnalog();

    pinMode(LED_BUILTIN, OUTPUT);

    while (1)
    {
        analogValue = analogRead(ADC_PIN3);

        digitalWrite(LED_BUILTIN, HIGH);
        delay(analogValue);

        digitalWrite(LED_BUILTIN, LOW);
        delay(analogValue);
    }

    return 0;
}
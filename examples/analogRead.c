#include "timing.h"
#include "analog.h"
#include "digital.h"
#include "arduino_uno.h"

/**
 * This example uses one of the Arduino UNO analog inputs
 * to read a value and delay the blink of the built-in led
 * based on the read value.
 * To run this example connect a potentiometer to the A3 pin
 * in Arduino UNO. If we turn the potentiometer one way the
 * led will blink faster. If we turn it the other way the led
 * will blink slower.
 */

int main()
{
    uint16_t analogValue = 0;

    initTiming();
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
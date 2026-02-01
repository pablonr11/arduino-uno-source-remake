#include "timing.h"
#include "analog.h"

/**
 * To run this example just connect a led
 * with a resistance to the pin 9 in the Arduino UNO.
 * The example will slowly increase the brightness
 * of the led. Once the max value is reached it will
 * slowly decrease the brightness.
 */

#define LED_PIN 9

uint8_t brightness = 0;
int8_t step = 1;

int main()
{
    initTiming();
    initPWM();

    while (1)
    {
        analogWrite(LED_PIN, brightness);

        brightness += step;

        if (brightness == 255)
            step = -1;
        else if (brightness == 0)
            step = 1;

        delay(10);
    }

    return 0;
}
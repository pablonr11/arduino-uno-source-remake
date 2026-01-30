#include "timing.h"
#include "analog.h"

#define LED_PIN 9

uint8_t brightness = 0;
int8_t step = 1;

int main()
{
    initTiming();
    initAnalog();

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
#include "timing.h"
#include "serial.h"

// This example uses serialPrintln to print
// the text "Hello from Arduino UNO :)" every 1 second
// to the serial.

int main()
{
    initTiming();
    serialBegin(9600);

    while (1)
    {
        serialPrintln("Hello from Arduino UNO :)");
        delay(1000);
    }

    return 0;
}
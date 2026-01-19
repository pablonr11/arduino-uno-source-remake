#include "timing.h"
#include "serial.h"
#include "digital.h"
#include "arduino_uno.h"

#include <string.h>

/**
 * In this example we use the serialRead and the serialAvailable
 * function to receive data from the Arduino UNO Serial.
 * If we connect the Arduino UNO through USB to a computer and
 * connect to it through serial we can send some commands.
 * The available commands are:
 * - ON: Turns on the built in led
 * - OFF: Turns off the built in led
 * - TOGGLE: Toggles the built in led
 * If we send something that is not recognized as a valid command
 * we print ""Unkown command: cmd" to the serial.
 * If the command exceds CMD_MAX_SIZE we print "Command too long"
 * and proceed to discard all the bytes until the EOL to properly
 * clean the rx buffer.
 */

#define CMD_MAX_SIZE 64

int main()
{
    int chr;
    char cmd[CMD_MAX_SIZE] = {0};
    uint8_t i = 0;
    uint8_t ledStatus = LOW;
    uint8_t discarding = 0;

    initTiming();
    serialBegin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    while (1)
    {
        while (serialAvailable() > 0)
        {
            chr = serialRead();
            if (chr == -1 || chr == '\r')
                continue;

            // If the previous command was too long we need to discard all the characters until the EOL.
            if (discarding)
            {
                if (chr == '\n')
                    discarding = 0;
                continue;
            }

            if (chr == '\n')
            {
                cmd[i] = '\0'; // Close string
                i = 0;

                if (strcmp(cmd, "ON") == 0)
                {
                    ledStatus = HIGH;
                    digitalWrite(LED_BUILTIN, ledStatus);
                }
                else if (strcmp(cmd, "OFF") == 0)
                {
                    ledStatus = LOW;
                    digitalWrite(LED_BUILTIN, ledStatus);
                }
                else if (strcmp(cmd, "TOGGLE") == 0)
                {
                    ledStatus = ledStatus == HIGH ? LOW : HIGH;
                    digitalWrite(LED_BUILTIN, ledStatus);
                }
                else
                {
                    serialPrint("Unkown command: ");
                    serialPrintln(cmd);
                }

                cmd[0] = '\0'; // Clean buffer
            }
            else
            {
                if (i >= CMD_MAX_SIZE - 1)
                {
                    i = 0;
                    serialPrintln("Command too long");
                    discarding = 1;
                }
                else
                {
                    cmd[i] = chr;
                    i++;
                }
            }
        }
    }

    return 0;
}
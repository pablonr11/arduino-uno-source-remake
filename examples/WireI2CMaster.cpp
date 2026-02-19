#include "timing.h"
#include "serial.h"
#include "digital.h"
#include "arduino_uno.h"
#include "utils.h"
#include "TwoWire.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// This example does the same as i2cMaster example
// but it is implemented using the Wire class

// If RTC is already configured just comment this line
// and upload the program again.
// This avoids configuring the RTC everytime on start.
#define SETUP_RTC 1

#define DS1307_I2C_ADDR 0x68
#define DS1307_SECONDS_ADDR 0x00
#define DS1307_MINUTES_ADDR 0x01
#define DS1307_HOURS_ADDR 0x02
#define DS1307_WEEKDAY_ADDR 0x03
#define DS1307_MONTHDAY_ADDR 0x04
#define DS1307_MONTH_ADDR 0x05
#define DS1307_YEAR_ADDR 0x06

void setupDS1307RTC(void);
void getDateTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

uint8_t readDS1307RTC(uint8_t *data, uint8_t dataLength);

int main()
{
    uint8_t readBytes = 0, year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    uint8_t data[7] = {0};
    char dateTimeStr[25];

    initTiming();
    Wire.begin();

    serialBegin(9600);

#ifdef SETUP_RTC
    setupDS1307RTC();
#endif

    while (1)
    {
        readBytes = readDS1307RTC(data, sizeof(data));

        if (readBytes == 0)
        {
            serialPrintln("0 bytes read from RTC");
        }
        else
        {
            second = BCD_TO_DEC(data[DS1307_SECONDS_ADDR] & 0x7F); // 0x7F removes the CH bit
            minute = BCD_TO_DEC(data[DS1307_MINUTES_ADDR]);
            hour = BCD_TO_DEC(data[DS1307_HOURS_ADDR] & 0x3F); // 0x3F because we use 24 hours format
            day = BCD_TO_DEC(data[DS1307_MONTHDAY_ADDR]);
            month = BCD_TO_DEC(data[DS1307_MONTH_ADDR]);
            year = BCD_TO_DEC(data[DS1307_YEAR_ADDR]);

            snprintf(
                dateTimeStr,
                sizeof(dateTimeStr),
                "20%02u-%02u-%02u %02u:%02u:%02u",
                year, month, day, hour, minute, second);

            serialPrintln(dateTimeStr);
        }

        delay(1000);
    }

    return 0;
}

uint8_t readDS1307RTC(uint8_t *data, uint8_t quantity)
{
    uint8_t readFromData = DS1307_SECONDS_ADDR;
    uint8_t writeStatus, readBytes = 0;

    Wire.beginTransmission(DS1307_I2C_ADDR);
    Wire.write(readFromData);
    writeStatus = Wire.endTransmission(0); // End and send repeated start to continue reading

    if (writeStatus != 0)
    {
        serialPrintln("Error while writing address to read from");
        return 0;
    }

    Wire.requestFrom(DS1307_I2C_ADDR, quantity);

    while (Wire.available())
    {
        int byte = Wire.read();
        if (byte == -1)
        {
            break;
        }

        data[readBytes] = (uint8_t)byte;
        readBytes++;
    }

    return readBytes;
}

void setupDS1307RTC(void)
{
    uint8_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

    serialPrintln("Starting DS1307 configuration");

    // __DATA__ and __TIME__ are calculated in compilation time
    // So once the RTC is configured the program should be
    // reflashed whit SETUP_RTC commented to avoid rewritting
    // the values
    serialPrintln(__DATE__);
    serialPrintln(__TIME__);
    serialPrintln(" ");

    getDateTime(&year, &month, &day, &hour, &minute, &second);

    // Data in the order we need to send them
    // First we send the address where we want to write.
    // Then the bytes in order.
    // The 0 is for the day of the week
    // The & 0b01111111 in seconds sets CH to 0 to enable de DS1307 clock
    uint8_t data[] = {
        DS1307_SECONDS_ADDR,
        DEC_TO_BCD(second) & 0b01111111,
        DEC_TO_BCD(minute),
        DEC_TO_BCD(hour),
        0,
        DEC_TO_BCD(day),
        DEC_TO_BCD(month),
        DEC_TO_BCD(year)};
    uint8_t dataLength = sizeof(data);

    Wire.beginTransmission(DS1307_I2C_ADDR);
    Wire.write(data, dataLength);
    uint8_t writeStatus = Wire.endTransmission();

    switch (writeStatus)
    {
    case 0:
        serialPrintln("DS1307 configured succesfully");
        break;
    case 1:
        serialPrintln("Error: Data length is to long for the buffer");
        break;
    case 2:
        serialPrintln("Error: NACK received after sending SLAW");
        break;
    case 3:
        serialPrintln("Error: NACK received after sending data");
        break;
    case 4:
        serialPrintln("Error: Arbitration lost");
        break;
    case 5:
        serialPrintln("Error: Unknown error");
        break;
    }

    while (writeStatus != 0)
        ;
}

void getDateTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second)
{
    // Get date in "MMM DD YYYY" Format
    char monthStr[4];
    int dayOfMonth = 0, yearFull = 0, intHour = 0, intMinute = 0, intSecond = 0;
    sscanf(__DATE__, "%s %d %d", monthStr, &dayOfMonth, &yearFull);

    // Convert month to number
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    *month = 0;
    for (int i = 0; i < 12; i++)
    {
        if (strcmp(monthStr, months[i]) == 0)
        {
            *month = i + 1;
            break;
        }
    }

    *year = (uint8_t)(yearFull % 100);
    *day = (uint8_t)(dayOfMonth);

    sscanf(__TIME__, "%d:%d:%d", &intHour, &intMinute, &intSecond);
    *hour = (uint8_t)intHour;
    *minute = (uint8_t)intMinute;
    *second = (uint8_t)intSecond;
}
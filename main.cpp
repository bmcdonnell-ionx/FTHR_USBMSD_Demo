#include "mbed.h"
#include "max32630fthr.h"
#include "USBMSD_SD.h"

MAX32630FTHR pegasus(MAX32630FTHR::VIO_3V3);

DigitalOut rLED(LED1);
DigitalOut gLED(LED2);

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{
    gLED = LED_ON;
    rLED = LED_ON;

    Thread::wait(100);

    USBMSD_SD sd(P0_5, P0_6, P0_4, P0_7);  // mosi, miso, sclk, cs
    rLED = LED_OFF;

    while (true) {
        gLED = !gLED;
        Thread::wait(500);
    }
}


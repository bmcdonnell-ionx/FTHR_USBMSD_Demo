#include "mbed.h"
#include "max32630fthr.h"
#include "USBMSD_SD.h"

DigitalOut rLED(LED1);
DigitalOut gLED(LED2);

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{
    gLED = LED_OFF;
    rLED = LED_ON;
    max32630fthrInit();
    Thread::wait(100);

    vddioh(P0_4, TRUE);
    vddioh(P0_5, TRUE);
    vddioh(P0_6, TRUE);
    vddioh(P0_7, TRUE);
    gLED = LED_ON;
    Thread::wait(100);

    USBMSD_SD sd(P0_5, P0_6, P0_4, P0_7);  // mosi, miso, sclk, cs
    rLED = LED_OFF;

    while (true) {
        gLED = !gLED;
        Thread::wait(500);
    }
}


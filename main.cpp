#include "mbed.h"
#include "USBMSD_BD.h"
#include "SDBlockDevice.h"
//#include "HeapBlockDevice.h"

#include <inttypes.h>

//#define BLOCK_SIZE   512
#define SDCARD_SPI_BUS_CLK_HZ  (10000000)

// Physical block device, can be any device that supports the BlockDevice API
// HeapBlockDevice bd(512*BLOCK_SIZE, BLOCK_SIZE);
SDBlockDevice bd(p5, p6, p7, p8, SDCARD_SPI_BUS_CLK_HZ);

// USB MSD 
USBMSD_BD msd(&bd);  

UARTSerial serial(USBTX, USBRX, 115200);


// redirect stdio to this serial port
FileHandle *mbed::mbed_override_console(int fd)
{
   return &serial;
}


// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{
    printf("\r\n--- Mbed OS SD card as USB MSD example ---\r\n");

    printf("Starting MSD... ");
    msd.disk_initialize();
    bool connectOK = msd.connect();
    printf("%s\r\n", (connectOK ? "OK" : "Fail :("));

    DigitalOut led1(LED1);

    while (true)
    {
        wait_ms(500);
        led1 = !led1;
    }
}

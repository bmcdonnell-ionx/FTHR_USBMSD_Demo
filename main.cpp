#include "mbed.h"
#include "USBMSD_BD.h"
#include "SDBlockDevice.h"
//#include "HeapBlockDevice.h"
#include "FATFileSystem.h"

//#define BLOCK_SIZE   512
#define SDCARD_SPI_BUS_CLK_HZ  (10000000)

// Physical block device, can be any device that supports the BlockDevice API
// HeapBlockDevice bd(512*BLOCK_SIZE, BLOCK_SIZE);
SDBlockDevice bd(p5, p6, p7, p8, SDCARD_SPI_BUS_CLK_HZ);

// File system declaration
FATFileSystem fs("fs");

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
    printf("--- Mbed OS filesystem example ---\n");

    Thread::wait(100);

    // Try to mount the filesystem
    printf("Mounting the filesystem... ");
    fflush(stdout);
    int err = fs.mount(&bd);
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        // Reformat if we can't mount the filesystem
        // this should only happen on the first boot
        printf("No filesystem found, formatting... ");
        fflush(stdout);
        err = fs.reformat(&bd);
        printf("%s\n", (err ? "Fail :(" : "OK"));
    }

    // Open the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);
    FILE *f = fopen("/fs/numbers.txt", "r+");
    printf("%s\n", (!f ? "Fail :(" : "OK"));
    if (!f) {
        // Create the numbers file if it doesn't exist
        printf("No file found, creating a new file... ");
        fflush(stdout);
        f = fopen("/fs/numbers.txt", "w+");
        printf("%s\n", (!f ? "Fail :(" : "OK"));

        for (int i = 0; i < 10; i++) {
            printf("\rWriting numbers (%d/%d)... ", i, 10);
            fflush(stdout);
            err = fprintf(f, "    %d\n", i);
            if (err < 0) {
                printf("Fail :(\n");
            }
        }
        printf("\rWriting numbers (%d/%d)... OK\n", 10, 10);

        printf("Seeking file... ");
        fflush(stdout);
        err = fseek(f, 0, SEEK_SET);
        printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    }

    // Go through and increment the numbers
    for (int i = 0; i < 10; i++) {
        printf("\rIncrementing numbers (%d/%d)... ", i, 10);
        fflush(stdout);

        // Get current stream position
        long pos = ftell(f);

        // Parse out the number and increment
        int32_t number;
        fscanf(f, "%d", &number);
        number += 1;

        // Seek to beginning of number
        fseek(f, pos, SEEK_SET);
    
        // Store number
        fprintf(f, "    %d\n", number);
    }
    printf("\rIncrementing numbers (%d/%d)... OK\n", 10, 10);

    // Close the file which also flushes any cached writes
    printf("Closing \"/fs/numbers.txt\"... ");
    fflush(stdout);
    err = fclose(f);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));

    // Display the root directory
    printf("Opening the root directory... ");
    fflush(stdout);
    DIR *d = opendir("/fs/");
    printf("%s\n", (!d ? "Fail :(" : "OK"));

    printf("root directory:\n");
    while (true) {
        struct dirent *e = readdir(d);
        if (!e) {
            break;
        }
        printf("    %s\n", e->d_name);
    }

    printf("Closing the root directory... ");
    fflush(stdout);
    err = closedir(d);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));

    // Display the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);
    f = fopen("/fs/numbers.txt", "r");
    printf("%s\n", (!f ? "Fail :(" : "OK"));

    printf("numbers:\n");
    while (!feof(f)) {
        int c = fgetc(f);
        printf("%c", c);
    }

    printf("\rClosing \"/fs/numbers.txt\"... ");
    fflush(stdout);
    err = fclose(f);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));




    // Switch to MSD
//    printf("Unmounting... ");
//    fflush(stdout);
//    err = fs.unmount();
//    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    

    printf("Starting MSD... ");
    msd.disk_initialize();
    err = msd.connect();
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));

    DigitalOut led1(LED1);

    while (true) {
        wait_ms(500);
        printf(".");        
        led1 = !led1;
    }
}


#include "mbed.h"

#define MAX14690_I2C_ADDR    0x50

DigitalOut led1(LED1);
I2C i2cm2(P5_7, P6_0);


// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{
    char data[5];
    data[0] = 0x14; // I2C address for first register (LDO2 CFG)
    data[1] = 0x00; // Dissable LDO2
    data[2] = 0x19; // Set voltage to 3.3V
    data[3] = 0x00; // Dissable LDO3
    data[4] = 0x19; // Set voltage to 3.3V
    i2cm2.write(MAX14690_I2C_ADDR, data, 5);
    data[1] = 0x02; // Enable LDO2
    data[3] = 0x02; // Enable LDO3
    i2cm2.write(MAX14690_I2C_ADDR, data, 5);

    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}


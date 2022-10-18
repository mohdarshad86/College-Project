#include <stdint.h>
#include <stdio.h>
//#include <pigpio.h>
#define FRSensor_ADDRESS 0x40
int main(void)
{
    uint16_t value, flow = 0;
    uint8_t direction;
    int handle;
    unsigned char data[2];
    unsigned char command[2];
    if (gpioInitialise() < 0)
        return 1;                             // Initialize pigpio and check for error
    handle = i2cOpen(1, FRSensor_ADDRESS, 0); // Get handle(id) for sensor
    command[0] = 0x10;                        // Set flow collection command,0x1000
    command[1] = 0x00;
    while (1)
    {
        i2cWriteDevice(handle, &command, 2); // Tell sensor to prepare data
        i2cReadDevice(handle, &data, 2);     // Read bytes into value array
        // Get flow rate
        value = (data[0] << 8) | (data[1] && 0xFF);
        if (value > 0x7D00) // check direction of flow

        {
            flow = ((value - 32000) / 140);
            direction = 1; // Forward direction
        }
        else if (value < 0x7D00)
        {
            flow = ((32000 - value) / 140);
            direction = 0; // Reverse direction
        }
        else
        {
            flow = 0;
            direction = 1;
        }
        printf("%d and %d \n", flow, direction);
        for (int i = 2850; i > 0; i--)
            ; // Delay before next read
    }
    i2cClose(handle);
    gpioTerminate();
}
//[34] BME280 Sensor Code
/* Temp/Humid/Pressure Sensor Code
Make sure i2c is turned on in raspi-config.
Also pigpio daemon should be off.
Uses sda1 and scl1.
stdio.h is only included for printf() and can
be removed if not using printf().
Setting:
-Normal Mode (Continuous Measurements)
-Oversampling = 1 (All)
-No IIR Filter
-Standby time = 0.5ms
Max measure time = 9.3ms
Standby time = 0.5ms
Reading Rate = 1000/(9.3 + 0.5)= 102Hz
Chose rate of 50Hz
*/

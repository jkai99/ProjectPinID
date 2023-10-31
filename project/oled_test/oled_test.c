#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C pins (adjust as needed)
#define I2C_PORT i2c0
#define SDA_PIN 2
#define SCL_PIN 3

// I2C address of the OLED (depends on your specific screen)
#define OLED_ADDRESS 0x3C

int main() {
    stdio_init_all();

    // Initialize the I2C hardware
    i2c_init(I2C_PORT, 100000);  // 100 kHz is a common I2C speed
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    // Try to read from the OLED address
    i2c_inst_t* i2c = i2c0;
    uint8_t data[1] = {0};

    if (i2c_read_blocking(i2c, OLED_ADDRESS, data, 1, false) == 1) {
        printf("I2C Detected on pins %d (SDA) and %d (SCL)\n", SDA_PIN, SCL_PIN);
    } else {
        printf("I2C Not Detected on pins %d (SDA) and %d (SCL)\n", SDA_PIN, SCL_PIN);
    }

    while (true) {
        // Your additional code can go here
    }

    return 0;
}
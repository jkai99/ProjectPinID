// Slave Code (Raspberry Pi Pico 1)
#include <stdio.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "pico/i2c_slave.h"

static const uint I2C_BAUDRATE = 100000; // 100 kHz

static struct {
    uint8_t mem[256];
    uint8_t mem_address;
    bool mem_address_written;
} context;

// Function to handle I2C events for a specific I2C slave address
void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {
        case I2C_SLAVE_RECEIVE:
            if (!context.mem_address_written) {
                context.mem_address = i2c_read_byte_raw(i2c);
                context.mem_address_written = true;
            } else {
                context.mem[context.mem_address] = i2c_read_byte_raw(i2c);
                context.mem_address++;
            }
            break;
        case I2C_SLAVE_REQUEST:
            i2c_write_byte_raw(i2c, context.mem[context.mem_address]);
            context.mem_address++;
            break;
        case I2C_SLAVE_FINISH:
            context.mem_address_written = false;
            break;
        default:
            break;
    }
}

int main() {
    stdio_init_all();
    puts("I2C Slave Example");

    // Configure I2C pins and their respective slave addresses
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_pull_up(4);

    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(5);

    gpio_set_function(6, GPIO_FUNC_I2C);
    gpio_pull_up(6);

    gpio_set_function(7, GPIO_FUNC_I2C);
    gpio_pull_up(7);

    // Set different slave addresses for GP4/GP5 and GP6/GP7
    i2c_slave_init(i2c0, 0x21, i2c_slave_handler); // GP4/GP5 will have address 0x17
    i2c_slave_init(i2c1, 0x17, i2c_slave_handler); // GP6/GP7 will have address 0x21


    while (1) {
        tight_loop_contents();
    }

    return 0;
}

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/i2c_slave.h"

// UART Constants
#define UART_BAUD_RATE 115200

// I2C Constants
static const uint I2C_BAUDRATE = 100000; // 100 kHz
static struct {
    uint8_t mem[256];
    uint8_t mem_address;
    bool mem_address_written;
} context;

// SPI Constants
#define BUF_LEN 128
uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];

// UART Function
void transmit_a_from_uart0() {
    uart_init(uart0, UART_BAUD_RATE);
    gpio_set_function(0, GPIO_FUNC_UART); // UART0 TX on GP0
    gpio_set_function(1, GPIO_FUNC_UART); // UART0 RX on GP1
    uart_set_baudrate(uart0, UART_BAUD_RATE);
    uart_putc(uart0, 'a');
}

// I2C Slave Function
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

void setup_i2c_slave() {
    i2c_init(i2c1, I2C_BAUDRATE);
    gpio_set_function(2, GPIO_FUNC_I2C); // I2C1 SDA on GP2
    gpio_set_function(3, GPIO_FUNC_I2C); // I2C1 SCL on GP3
    i2c_slave_init(i2c1, 0x21, i2c_slave_handler);
}

// SPI Function
void spi_communication() {
    spi_init(spi1, 1 * 1000000);
    gpio_set_function(10, GPIO_FUNC_SPI); // SPI1 SCK on GP10
    gpio_set_function(11, GPIO_FUNC_SPI); // SPI1 MOSI on GP11
    gpio_set_function(12, GPIO_FUNC_SPI); // SPI1 MISO on GP12
    gpio_set_function(13, GPIO_FUNC_SPI); // SPI1 CS on GP13

    out_buf[0] = 100; // Example data
    spi_write_blocking(spi1, out_buf, 1);
    spi_read_blocking(spi1, 0, in_buf, 1);
}

int main() {
    stdio_init_all();
    setup_i2c_slave();

    while (1) {
        transmit_a_from_uart0();
        spi_communication();
        sleep_ms(1000); // Adjust as needed
    }

    return 0;
}

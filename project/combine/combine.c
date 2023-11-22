#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

// Constants for UART configuration
#define BAUD_RATE 115200

// Define GPIO pins for buttons
#define UART_BUTTON 20          // Check for UART
#define I2C_BUTTON 21           // Check for I2C
#define SPI_BUTTON 22           // check for SPI

// Function to check for button press
bool is_button_pressed(int pin) {
    return gpio_get(pin) == 0;
}

// Function to receive data on GP1 for UART0
void uart_rx_code() {
    uart_init(uart0, BAUD_RATE);
    gpio_set_function(1, GPIO_FUNC_UART);  // Configure GP1 for UART

    while (true) {
        // Check for received characters
        while (uart_is_readable(uart0)) {
            char receivedChar = uart_getc(uart0);

            if ((receivedChar >= 32 && receivedChar <= 126) || receivedChar == 10 || receivedChar == 13) {
                printf("Received data on GP1 (UART RX pin 1): %c\n", receivedChar);
                // Add your specific handling code for GP1 here
            } else {
                printf("Received an unexpected character on GP1 (UART RX pin 1). Likely to be SPI TX.\n");
            }
        }
    }
}

// Function to sweep through all 7-bit I2C addresses
void i2c_master_code() {
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if ((addr & 0x78) == 0 || (addr & 0x78) == 0x78)
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
}

// Function to handle SPI communication
void spi_code() {
    // Enable SPI0 at 1 MHz
    spi_init(spi_default, 1 * 1000000);

    gpio_set_function(18, GPIO_FUNC_SPI); // SCK (Clock)
    gpio_set_function(19, GPIO_FUNC_SPI); // SDO0/MISO (Master In Slave Out)
    gpio_set_function(16, GPIO_FUNC_SPI); // SDI0/MOSI (Master Out Slave In)
    gpio_set_function(17, GPIO_FUNC_SPI); // CSN (Chip Select)

    // We need two buffers, one for the data to send, and one for the data to receive.
    uint8_t out_buf[1], in_buf[1];

    // Initialize the buffers to 0.
    out_buf[0] = 0;
    in_buf[0] = 0;

    for (uint8_t i = 100; ; ++i) {
        printf("Sending data %d to SPI Peripheral\n", i);
        out_buf[0] = i;

        // Write the output buffer to MOSI
        spi_write_blocking(spi_default, out_buf, 1);

        // Wait for some time to allow the slave to respond
        sleep_ms(100);

        // Read from MISO to the input buffer
        spi_read_blocking(spi_default, 0, in_buf, 1);

        // Print received data
        printf("Received data: %d\n", in_buf[0]);

        // Sleep for some seconds so you get a chance to read the output.
        sleep_ms(2 * 1000);
    }
}

int main() {
    // Set up UART
    stdio_init_all();

    // Set GPIO pins as inputs with pull-down resistors
    gpio_set_dir(UART_BUTTON, GPIO_IN);
    gpio_pull_down(UART_BUTTON);

    gpio_set_dir(I2C_BUTTON, GPIO_IN);
    gpio_pull_down(I2C_BUTTON);

    gpio_set_dir(SPI_BUTTON, GPIO_IN);
    gpio_pull_down(SPI_BUTTON);

    bool i2cInitialized = false;

    while (1) {
        // Check if UART button is pressed
        if (is_button_pressed(UART_BUTTON)) {
            printf("Start checking for UART pins\n");
            uart_rx_code();
            // You might want to debounce the button press here
            sleep_ms(100);
        }

        // Check if I2C button is pressed
        if (is_button_pressed(I2C_BUTTON)) {
            printf("Start checking for I2C pins\n");

            // I2C initialization code
            if (!i2cInitialized) {
                #if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
                    #warning i2c/bus_scan example requires a board with I2C pins
                    puts("Default I2C pins were not defined");
                #else
                    i2c_init(i2c_default, 100 * 1000);
                    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
                    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
                    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
                    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
                    // No need for bi_decl or bi_2pins_with_func here
                    i2cInitialized = true;
                #endif
            }


            i2c_master_code();
            // You might want to debounce the button press here
            sleep_ms(100);
        }

        // Check if SPI button is pressed
        if (is_button_pressed(SPI_BUTTON)) {
            printf("Start checking for SPI pins\n");
            spi_code();
            // You might want to debounce the button press here
            sleep_ms(100);
        }

        // Other main loop tasks
    }

    return 0;
}

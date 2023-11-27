#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

// Constants for UART configuration
// #define BAUD_RATE 115200

#define BUF_LEN 128

// Define GPIO pins for buttons
#define UART_BUTTON 20          // Check for UART
#define I2C_BUTTON 21           // Check for I2C
#define SPI_BUTTON 22           // check for SPI

// Constants for UART configuration
#define GPIO_PIN_GP1 1

// Function to check for button press
bool is_button_pressed(int pin) {
    return gpio_get(pin) == 0;
}

// Function to get the next baud rate from the array
uint getNextBaudRate(uint currentBaudRate);

// Function to receive data on a specified UART and GPIO pin
void receive_on_gpio_uart(int uart_instance, int gpio_pin, uint baud_rate) {
    // Get the UART instance based on the specified uart_instance
    uart_inst_t *uart = uart_instance == 0 ? uart0 : uart1;  // Get the UART instance

    // Initialize UART with the specified baud rate and configure GPIO pin
    uart_init(uart, baud_rate);
    gpio_set_function(gpio_pin, GPIO_FUNC_UART);

    // Print information about the current testing configuration
    printf("Testing GP%d (UART%d RX pin %d) at baud rate %d...\n", gpio_pin, uart_instance, gpio_pin, baud_rate);

    while (true) {
        uint64_t start_time = time_us_64();

        // Check for received characters for 5 seconds
        while (time_us_64() - start_time < 5 * 1000000) {
            if (uart_is_readable(uart)) {
                // Read and print received character
                char receivedChar = uart_getc(uart);
                printf("Received data: %c\n", receivedChar);
            }
        }

        // Move to the next baud rate
        printf("Testing complete at baud rate %d\n", baud_rate);
        printf("Sleeping for 5 seconds before testing the next baud rate...\n");
        sleep_ms(5000);

        // Move to the next baud rate in the array
        baud_rate = getNextBaudRate(baud_rate);  // Implement this function to get the next baud rate
        // Deinitialize UART and reinitialize with the new baud rate
        uart_deinit(uart);
        uart_init(uart, baud_rate);
    }
}

// Function to get the next baud rate from the array
uint getNextBaudRate(uint currentBaudRate) {
    const uint baud_rates[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 74880, 115200, 230400, 250000};
    for (int i = 0; i < sizeof(baud_rates) / sizeof(baud_rates[0]); i++) {
        if (baud_rates[i] == currentBaudRate) {
            // Return the next baud rate in the array
            return baud_rates[(i + 1) % (sizeof(baud_rates) / sizeof(baud_rates[0]))];
        }
    }
    // If the current baud rate is not found, return the first baud rate in the array
    return baud_rates[0];
}


// // Function to receive data on GP1 for UART0
// void uart_rx_code() {
//     uart_init(uart0, BAUD_RATE);
//     gpio_set_function(1, GPIO_FUNC_UART);  // Configure GP1 for UART

//     while (true) {
//         // Check for received characters
//         while (uart_is_readable(uart0)) {
//             char receivedChar = uart_getc(uart0);

//             printf("Received data on GP1 (UART RX pin 1): %c\n", receivedChar);

//             // if ((receivedChar >= 32 && receivedChar <= 126) || receivedChar == 10 || receivedChar == 13) {
//             //     printf("Received data on GP1 (UART RX pin 1): %c\n", receivedChar);
//             //     // Add your specific handling code for GP1 here
//             // } else {
//             //     printf("Received an unexpected character on GP1 (UART RX pin 1). Likely to be SPI Pin.\n");
//             // }
//         }

//         // Add a small delay to prevent busy-waiting
//         sleep_ms(10);
//     }
// }

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
    spi_set_slave(spi_default, true);

    gpio_set_function(18, GPIO_FUNC_SPI); // SCK (Clock)
    gpio_set_function(19, GPIO_FUNC_SPI); // SDO0/MISO (Master In Slave Out)
    gpio_set_function(16, GPIO_FUNC_SPI); // SDI0/MOSI (Master Out Slave In)
    gpio_set_function(17, GPIO_FUNC_SPI); // CSN (Chip Select)

    // We need two buffers, one for the data to send, and one for the data to receive.
    // uint8_t out_buf[1], in_buf[1];
    uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];

      // Initialize output buffer
    for (uint8_t i = 0; i < BUF_LEN; ++i)
    {
        out_buf[i] = 0;
        in_buf[i] = 0;
    }

    // // Initialize the buffers to 0.
    // out_buf[0] = 0;
    // in_buf[0] = 0;

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
        // sleep_ms(2 * 1000);
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
            // uart_rx_code();
            // You might want to debounce the button press here
            uint initial_baud_rate = 300;  // Start with the first baud rate
            printf("Testing UART0 on GP1:\n");
            receive_on_gpio_uart(0, GPIO_PIN_GP1, initial_baud_rate);
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

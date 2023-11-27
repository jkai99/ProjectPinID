#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Constants for UART configuration
#define GPIO_PIN_GP1 1

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

int main() {
    stdio_init_all();

    uint initial_baud_rate = 300;  // Start with the first baud rate
    printf("Testing UART0 on GP1:\n");
    receive_on_gpio_uart(0, GPIO_PIN_GP1, initial_baud_rate);

    return 0;
}

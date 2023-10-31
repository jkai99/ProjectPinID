#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"

// Constants for UART configuration
#define BAUD_RATE 115200

// Define the GPIO pins to be tested
const uint gp_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 26, 27, 28};

// Function to test a GPIO pin
bool test_gp_pin(uint pin) {
    gpio_init(pin);                     // Initialize the GPIO pin
    gpio_set_dir(pin, GPIO_OUT);        // Set the pin direction to output
    gpio_put(pin, 1);                   // Set the pin high (1) 
    sleep_ms(1000);                     // and wait for 1 second
    gpio_put(pin, 0);                   // Set the pin low (0)

    // You can add some additional checks here to determine if the pin is working
    bool is_working = true;             // You should add checks to set this to false if the pin doesn't work

    return is_working;
}

// Function to test UART loopback
void test_uart_loopback() {
    // Define UART instance (UART0) and associated pins for testing
    uart_inst_t *uart = uart0;
    int tx_pin = 0;         // UART0 TX on GPIO 0
    int rx_pin = 1;         // UART0 RX on GPIO 1

    // Initialize UART communication with the specified baud rate
    uart_init(uart, BAUD_RATE);

    // Configure GPIO pins for UART communication
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);

    bool dataMatched = false;

    while (!dataMatched) {
        // Check if UART TX is writable
        if (uart_is_writable(uart)) {
            // Transmit 'a'
            char sendChar = 'a';
            uart_putc(uart, sendChar);
            printf("UART0 Sent: %c\n", sendChar);
            printf("PIN 1: Working\n");
            sleep_ms(2000);
        }

        // Check for received characters
        while (uart_is_readable(uart)) {
            char receivedChar = uart_getc(uart);
            printf("UART0 Received: %c\n", receivedChar);
            printf("PIN 2: Working\n");
            sleep_ms(2000);

            if (receivedChar == 'a') {
                dataMatched = true; // Data sent and received match, exit the loop
                break;
            }
        }
        if (dataMatched) {
        printf("Both Pin 1 and Pin 2 for UART 0 are working!\n");
    }
    }
}


int main() {
    stdio_init_all();

    printf("Testing GPIO Pins:\n");

    bool all_pins_working = true;

    for (uint i = 0; i < sizeof(gp_pins) / sizeof(gp_pins[0]); i++) {
        uint pin = gp_pins[i];
        printf("GP%d: ", pin);
        
        if (test_gp_pin(pin)) {
            printf("Working\n");
        } else {
            printf("Not Working\n");
            all_pins_working = false;
        }
    }
    
    if (all_pins_working) {
        printf("All GP pins are working\n");
    }

    printf("Waiting for 10 seconds before testing UART...\n");
    sleep_ms(10000);


    printf("Testing UART Loopback:\n");
    test_uart_loopback();

    return 0;
}

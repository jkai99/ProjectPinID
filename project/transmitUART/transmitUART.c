#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "pico/time.h"

// Constants for UART configuration
#define BAUD_RATE 115200

// Function to transmit 'a' from a specific GP pin
void transmit_a_from_pin(int gpio_pin) {
    // Initialize UART communication with the specified baud rate for UART0
    uart_init(uart0, BAUD_RATE);

    // Configure the GPIO pin for UART transmission (make sure to select the correct UART module)
    gpio_set_function(gpio_pin, GPIO_FUNC_UART);
    uart_set_baudrate(uart0, BAUD_RATE);

    // Check if UART TX is writable
    if (uart_is_writable(uart0)) {
        // Transmit 'a' from the current GPIO pin
        uart_putc(uart0, 'a');
    } else {
        printf("GP%d is not UART TX - Not transmitting.\n", gpio_pin);
    }
}

int main() {
    stdio_init_all();

    printf("Transmitting 'a' from specified GP pins simultaneously:\n");

    // Define an array of GP pins to transmit from
    int tx_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 26, 27, 28};
    int num_pins = sizeof(tx_pins) / sizeof(tx_pins[0]);

    // Initialize UART communication with the specified baud rate for UART0 once
    uart_init(uart0, BAUD_RATE);

    while (1) {
        for (int i = 0; i < num_pins; i++) {
            int gpio_pin = tx_pins[i];
            printf("Transmitting 'a' from GP%d\n", gpio_pin);
            transmit_a_from_pin(gpio_pin);
        }
        sleep_ms(1000); // Wait for 1 second before transmitting again
        
        // Check the state of GPIO pins and print "Not UART TX" if not transmitting
        for (int i = 0; i < num_pins; i++) {
            int gpio_pin = tx_pins[i];
            if (gpio_get(gpio_pin) == 0) {
                printf("GP%d is not UART TX - Not transmitting.\n", gpio_pin);
            }
        }
    }

    return 0;
}

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "pico/time.h"

// Constants for UART configuration
#define BAUD_RATE 115200

// Function to transmit 'a' from a specific GP pin for UART0
void transmit_a_from_pin_uart0(int gpio_pin) {
    // Initialize UART communication with the specified baud rate for UART0
    uart_init(uart0, BAUD_RATE);

    // Configure the GPIO pin for UART0 transmission
    gpio_set_function(gpio_pin, GPIO_FUNC_UART);
    uart_set_baudrate(uart0, BAUD_RATE);

    // Check if UART0 TX is writable
    if (uart_is_writable(uart0)) {
        // Transmit 'a' from the current GPIO pin using UART0
        uart_putc(uart0, 'a');
    } else {
        printf("GP%d is not UART0 TX - Not transmitting.\n", gpio_pin);
    }
}

// Function to transmit 'a' from a specific GP pin for UART1
void transmit_a_from_pin_uart1(int gpio_pin) {
    // Initialize UART communication with the specified baud rate for UART1
    uart_init(uart1, BAUD_RATE);

    // Configure the GPIO pin for UART1 transmission
    gpio_set_function(gpio_pin, GPIO_FUNC_UART);
    uart_set_baudrate(uart1, BAUD_RATE);

    // Check if UART1 TX is writable
    if (uart_is_writable(uart1)) {
        // Transmit 'a' from the current GPIO pin using UART1
        uart_putc(uart1, 'a');
    } else {
        printf("GP%d is not UART1 TX - Not transmitting.\n", gpio_pin);
    }
}

int main() {
    stdio_init_all();

    printf("Transmitting 'a' from specified GP pins to UART0 and UART1 simultaneously:\n");

    // Define an array of GP pins to transmit from
    int tx_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 26, 27, 28};
    int num_pins = sizeof(tx_pins) / sizeof(tx_pins[0]);

    // Initialize UART communication with the specified baud rate for UART0 and UART1 once
    uart_init(uart0, BAUD_RATE);
    uart_init(uart1, BAUD_RATE);

    while (1) {
        for (int i = 0; i < num_pins; i++) {
            int gpio_pin = tx_pins[i];
            printf("Transmitting 'a' from GP%d to UART0\n", gpio_pin);
            transmit_a_from_pin_uart0(gpio_pin);
            printf("Transmitting 'a' from GP%d to UART1\n", gpio_pin);
            transmit_a_from_pin_uart1(gpio_pin);
        }
        sleep_ms(1000); // Wait for 1 second before transmitting again
    }

    return 0;
}

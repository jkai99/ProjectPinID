#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Constants for UART configuration
#define BAUD_RATE 115200

// Function to test UART transmission from both GPIO 0 and GPIO 4
void test_uart_transmit_from_gpio0_and_gpio4() {
    // Define UART instances for both UART0 (GPIO 0) and UART1 (GPIO 4)
    uart_inst_t *uart0_instance = uart0;
    uart_inst_t *uart1_instance = uart1;
    int tx_pin0 = 0; // UART0 TX on GPIO 0
    int tx_pin1 = 4; // UART1 TX on GPIO 4

    // Initialize UART communication with the specified baud rate for both UARTs
    uart_init(uart0_instance, BAUD_RATE);
    uart_init(uart1_instance, BAUD_RATE);

    // Configure the GPIO pins for UART transmission
    gpio_set_function(tx_pin0, GPIO_FUNC_UART);
    gpio_set_function(tx_pin1, GPIO_FUNC_UART);

    while (true) {
        // Check if UART0 TX is writable
        if (uart_is_writable(uart0_instance)) {
            // Transmit 'a' from GPIO 0
            char sendChar = 'a';
            uart_putc(uart0_instance, sendChar);
            // printf(" %c\n", sendChar);
            printf("a");
        }

        // Check if UART1 TX is writable
        if (uart_is_writable(uart1_instance)) {
            // Transmit 'a' from GPIO 4
            char sendChar = 'b';
            uart_putc(uart1_instance, sendChar);
            // printf(" %c\n", sendChar);
            printf("b");
        }
        
        sleep_ms(2000);
    }
}

int main() {
    stdio_init_all();

    printf("Testing UART Transmission from GPIO 0 and GPIO 4:\n");

    // Call the function to transmit from both GPIO 0 and GPIO 4
    test_uart_transmit_from_gpio0_and_gpio4();

    return 0;
}

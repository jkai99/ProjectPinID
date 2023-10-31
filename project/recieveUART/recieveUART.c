#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Constants for UART configuration
#define BAUD_RATE 115200

// Function to receive data on GP1
void receive_on_gp1() {
    uart_init(uart0, BAUD_RATE);
    gpio_set_function(1, GPIO_FUNC_UART);  // Configure GP1 for UART

    while (true) {
        // Check for received characters
        while (uart_is_readable(uart0)) {
            char receivedChar = uart_getc(uart0);
            printf("Received data on GP1 (UART RX pin 1): %c\n", receivedChar);
            // Add your specific handling code for GP1 here
        }
    }
}

int main() {
    stdio_init_all();

    printf("Receiving data on GP1 (UART RX pin 1):\n");

    receive_on_gp1();

    return 0;
}

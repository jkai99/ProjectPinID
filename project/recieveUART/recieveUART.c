#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Constants for UART configuration
#define BAUD_RATE 115200

// Function to test UART reception and differentiate based on the UART RX pin
void test_uart_receive(int expected_rx_pin) {
    uart_inst_t *uart;
    
    switch (expected_rx_pin) {
        case 1:
            uart = uart0;
            break;
        case 5:
            uart = uart1;
            break;
        // case 9:
        //     uart = uart2;
        //     break;
        // case 13:
        //     uart = uart3;
        //     break;
        // case 17:
        //     uart = uart4;
        //     break;
        default:
            uart = uart0; // Use UART0 as the default if the expected pin is not matched
            break;
    }

    // Initialize UART communication with the specified baud rate
    uart_init(uart, BAUD_RATE);
    gpio_set_function(expected_rx_pin, GPIO_FUNC_UART);

    while (true) {
        // Check for received characters
        while (uart_is_readable(uart)) {
            char receivedChar = uart_getc(uart);
            printf("Received data on UART RX pin %d: %c\n", expected_rx_pin, receivedChar);

            // Use an if/else statement to differentiate based on the UART RX pin
            if (expected_rx_pin == 1) {
                // Take specific actions for data received on pin 1
                printf("Pin 1\n");
                sleep_ms(1000);
            } else if (expected_rx_pin == 5) {
                // Take specific actions for data received on pin 5
                printf("Pin 5\n");
                sleep_ms(1000);
            } else if (expected_rx_pin == 9) {
                // Take specific actions for data received on pin 9
                printf("Pin 9\n");
                sleep_ms(1000);
            } else if (expected_rx_pin == 13) {
                // Take specific actions for data received on pin 13
                printf("Pin 13\n");
                sleep_ms(1000);
            } else if (expected_rx_pin == 17) {
                // Take specific actions for data received on pin 17
                printf("Pin 17\n");
                sleep_ms(1000);
            }
        }
    }
}

int main() {
    stdio_init_all();

    printf("Testing UART Reception:\n");

    // Specify the expected UART RX pin (1, 5, 9, 13, or 17)
    int expected_rx_pin = 1;  // Change this to the desired RX pin
    
    test_uart_receive(expected_rx_pin);

    return 0;
}

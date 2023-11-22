#include <stdio.h>
#include "pico/stdlib.h"

// Constants for UART configuration
#define UART_ID uart1
#define BAUD_RATE 115200

// GPIO pins for UART communication
#define UART_TX_PIN 4  // GP16 for Transmitting

// Function to initialize UART and GPIO pins
void initialize() {
    // Initialize standard input/output
    stdio_init_all();
    
    // Initialize UART communication with the specified baud rate
    uart_init(UART_ID, BAUD_RATE);
    
    // Configure GPIO pins for UART communication
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
}

int main() {
    // Initialize UART and GPIO
    initialize();

    // Infinite loop to continuously cycle through the alphabet
    while (1) {
        for (char currentCharToSend = 'A'; currentCharToSend <= 'Z'; ++currentCharToSend) {
            // Transmit the current character
            uart_putc(UART_ID, currentCharToSend);

            // Sleep for a short duration
            sleep_ms(1000);
        }
    }

            
    return 0;
}

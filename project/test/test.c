#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

int main() {
    stdio_init_all();

    printf("Testing UART TX Pins:\n");

    // Define the pins you want to check
    const uint uart_tx_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 26, 27, 28};

    for (uint i = 0; i < sizeof(uart_tx_pins) / sizeof(uart_tx_pins[0]); i++) {
        uint pin = uart_tx_pins[i];
        printf("GP%d: ", pin);
        
        int function = gpio_get_function(pin);

        if (function == GPIO_FUNC_UART) {
            printf("UART TX\n");
        } else {
            printf("Not UART TX\n");
        }
    }
    
    return 0;
}

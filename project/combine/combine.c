#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

// Constants for UART and I2C configuration
#define UART_BAUD_RATE 115200

// UART setup function
void setup_uart() {
    uart_init(uart0, UART_BAUD_RATE);
    gpio_set_function(0, GPIO_FUNC_UART);  // Use GP0 for UART
    gpio_set_function(1, GPIO_FUNC_UART);  // Use GP1 for UART
    uart_set_hw_flow(uart0, false, false); // No hardware flow control
    uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
}

// State variable to keep track of which task is running
volatile bool is_uart_task = true;

void check_uart_on_gp1() {
    setup_uart();

    while (true) {
        if (is_uart_task) {
            // Check for received characters
            while (uart_is_readable(uart0)) {
                char receivedChar = uart_getc(uart0);

                if ((receivedChar >= 32 && receivedChar <= 126) || receivedChar == 10 || receivedChar == 13) {
                    printf("Received data on UART0: %c\n", receivedChar);
                    // Add your specific handling code here
                } else {
                    printf("Received an unexpected character on UART0.\n");
                }
            }
            // Switch to the other task
            is_uart_task = false;
        }
    }
}

bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void check_i2c_pins() {
    // Initialize I2C with the default pins
    i2c_init(i2c_default, 100 * 1000);

    while (true) {
        if (!is_uart_task) {
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
                if (reserved_addr(addr))
                    ret = PICO_ERROR_GENERIC;
                else
                    ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

                printf(ret < 0 ? "." : "@");
                printf(addr % 16 == 15 ? "\n" : "  ");
            }
            printf("Done.\n");

            // Switch to the other task
            is_uart_task = true;
        }
    }
}

int main() {
    stdio_init_all();

    while (true) {
        // Both tasks run sequentially in a cooperative manner
        check_uart_on_gp1();
        check_i2c_pins();
    }

    return 0;
}

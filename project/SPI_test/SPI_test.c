#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

int main() {
    stdio_init_all();

    // Set up the SPI interface (using SPI0)
    spi_init(spi0, 1000000); // SPI0 with a baud rate of 1 MHz

    // Define the SPI pins
    gpio_set_function(2, GPIO_FUNC_SPI); // SPI0 SCK (GP2)
    gpio_set_function(3, GPIO_FUNC_SPI); // SPI0 TX (GP3)
    gpio_set_function(4, GPIO_FUNC_SPI); // SPI0 RX (GP4)
    gpio_set_function(5, GPIO_FUNC_SPI); // SPI0 CSn (GP5)

    // Configure the SPI settings for your LED screen
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    // Initialize and configure any additional control pins, e.g., reset, backlight
    gpio_init(6); // Define and configure GPIO pin for reset
    gpio_set_dir(6, GPIO_OUT);
    gpio_put(6, 1); // Set the reset pin high initially

    // Your display-specific initialization code goes here
    // You may need to send initialization commands to your LED screen

    while (1) {
        // Your SPI data to send to the LED screen
        uint8_t data_to_send = 0x55;

        // Select the LED screen by pulling its CSn (Chip Select) pin low
        gpio_put(5, 0);

        // Send data over SPI
        spi_write_blocking(spi0, &data_to_send, 1);

        // Deselect the LED screen by pulling its CSn pin high
        gpio_put(5, 1);

        sleep_ms(1000); // Sleep for 1 second before sending the next data
    }

    return 0;
}

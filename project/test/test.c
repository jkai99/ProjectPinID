#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define CLOCK_PIN 2

volatile uint32_t last_rising_edge_time = 0;
volatile uint32_t time_difference = 0;

// Interrupt Service Routine
void rising_edge_isr(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    if (last_rising_edge_time != 0) {
        time_difference = current_time - last_rising_edge_time;
    }
    last_rising_edge_time = current_time;
}

int main() {
    stdio_init_all();

    // Initialize GPIO for clock input
    gpio_init(CLOCK_PIN);
    gpio_set_dir(CLOCK_PIN, GPIO_IN);
    gpio_pull_up(CLOCK_PIN);  // Optional, based on your circuit

    // Attach the ISR to the GPIO pin
    gpio_set_irq_enabled_with_callback(CLOCK_PIN, GPIO_IRQ_EDGE_RISE, true, &rising_edge_isr);

    while (true) {
        if (time_difference != 0) {
            printf("Time between rising edges: %u us\n", time_difference);
            time_difference = 0;  // Reset after printing
        }
        sleep_ms(1000);  // Delay to avoid printing too rapidly
    }

    return 0;
}
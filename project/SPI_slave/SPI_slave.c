#include <stdio.h>
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#define BUF_LEN 128

void printbuf(uint8_t buf[], size_t len)
{
  int i;
  for (i = 0; i < len; ++i)
  {
    if (i % 16 == 15)
      printf("%02x\n", buf[i]);
    else
      printf("%02x ", buf[i]);
  }

  // append trailing newline if there isn't one
  if (i % 16)
  {
    putchar('\n');
  }
}

int main()
{
  // Enable UART so we can print
  stdio_init_all();
  sleep_ms(2 * 1000);
  printf("SPI Bidirectional Communication\n");

  // Enable SPI 0 at 1 MHz and connect to GPIOs
  spi_init(spi_default, 1 * 1000000);
  spi_set_slave(spi_default, true);

  gpio_set_function(18, GPIO_FUNC_SPI); // SCK (Clock)
  gpio_set_function(19, GPIO_FUNC_SPI); // SDO0/MISO (Master In Slave Out)
  gpio_set_function(16, GPIO_FUNC_SPI); // SDI0/MOSI (Master Out Slave In)
  gpio_set_function(17, GPIO_FUNC_SPI); // CSN (Chip Select)

  // Alternate pins 2,3,4,5
  // gpio_set_function(2, GPIO_FUNC_SPI); // SCK (Clock)
  // gpio_set_function(3, GPIO_FUNC_SPI); // SDO0/MISO (Master In Slave Out)
  // gpio_set_function(4, GPIO_FUNC_SPI); // SDI0/MOSI (Master Out Slave In)
  // gpio_set_function(5, GPIO_FUNC_SPI); // CSN (Chip Select)

  uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];

  // Initialize output buffer
  for (uint8_t i = 0; i < BUF_LEN; ++i)
  {
    out_buf[i] = 0;
    in_buf[i] = 0;
  }

  for (uint8_t i = 1;; ++i)
  {
    printf("Sending data %d to SPI Peripheral\n", i);
    out_buf[0] = i;
    // Check if master is ready to send
    if (spi_is_writable(spi_default))
    {
      printf("Reading data from SPI..\n");

      // Read from MISO to the input buffer
      spi_read_blocking(spi_default, 0, in_buf, 1);

      // Print received data
      printf("Received data: %d\n", in_buf[0]);

      // Increment the received data and send it back to the master
      // out_buf[0] = in_buf[0] + 1;

      // Write the output buffer to MOSI
      spi_write_blocking(spi_default, out_buf, 1);
    }
  }
}

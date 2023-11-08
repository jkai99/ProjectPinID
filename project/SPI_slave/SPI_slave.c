
#include <stdio.h>
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#define BUF_LEN 128


void printbuf (uint8_t buf [], size_t len) {
  int i;
  for (i = 0; i < len; ++i) {
    if (i % 16 == 15)
      printf ("%02x\n", buf [i]);
    else
      printf ("%02x ", buf [i]);
  }

  // append trailing newline if there isn't one
  if (i % 16) {
    putchar ('\n');
  }
}


int main() {
  // Enable UART so we can print
  stdio_init_all();
  sleep_ms (2 * 1000);
  printf ("SPI Peripheral Example\n");

  // Enable SPI 0 at 1 MHz and connect to GPIOs
  spi_init (spi_default, 1 * 1000000);
  spi_set_slave (spi_default, true);

  gpio_set_function (PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function (PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function (PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  gpio_set_function (PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);

  // // Make the SPI pins available to picotool
  // bi_decl ( bi_4pins_with_func (
  //     PICO_DEFAULT_SPI_RX_PIN,
  //     PICO_DEFAULT_SPI_TX_PIN,
  //     PICO_DEFAULT_SPI_SCK_PIN,
  //     PICO_DEFAULT_SPI_CSN_PIN,
  //     GPIO_FUNC_SPI
  //   )
  // );

  uint8_t out_buf [BUF_LEN], in_buf [BUF_LEN];

  // Initialize output buffer
  for (uint8_t i = 0; i < BUF_LEN; ++i) {
    out_buf [i] = 0;
    in_buf [i] = 0;
  }

  // printbuf (out_buf, BUF_LEN);

  // for (uint8_t i = 0; ; ++i) {
  //   printf ("Reading data from SPI..\n");
  //   // Write the output buffer to MOSI, and at the same time read from MISO.
  //   spi_write_read_blocking (spi_default, out_buf, in_buf, 1);

  //   // Write to stdio whatever came in on the MISO line.
  //   if (in_buf [0] != 0) {
  //     printf ("Data received: %d\n", in_buf [0]);
  //   }
  //   else {
  //     printf ("No data received\n");
  //   }
  // }

  while (1) {
    if (spi_is_readable(spi_default)) {
      printf ("Reading data from SPI..\n");
      // Write the output buffer to MOSI, and at the same time read from MISO.
      spi_read_blocking (spi_default, 0, in_buf, 1);

      printf ("Data received: %d\n", in_buf [0]);
    }
  }
}


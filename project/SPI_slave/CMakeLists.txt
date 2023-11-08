add_executable(SPI_slave
        SPI_slave.c
        )

# Pull in basic dependencies
target_link_libraries(SPI_slave pico_stdlib hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(SPI_slave)

# add url via pico_set_program_url
example_auto_set_url(SPI_slave)

if (TARGET tinyusb_device)
        # enable usb output
        pico_enable_stdio_usb(SPI_slave 1)
elseif(PICO_ON_DEVICE)
        message(WARNING "not building SPI_slave because TinyUSB submodule is not initialized in the SDK")
endif()

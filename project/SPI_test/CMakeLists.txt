if (TARGET tinyusb_device)
    add_executable(SPI_test 
    SPI_test.c
            )

    # pull in common dependencies
    target_link_libraries(SPI_test pico_stdlib hardware_i2c hardware_spi hardware_uart)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(SPI_test 1)
    pico_enable_stdio_uart(SPI_test 1) #changed from 0 to 1

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(SPI_test)

    # add url via pico_set_program_url
    example_auto_set_url(SPI_test)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

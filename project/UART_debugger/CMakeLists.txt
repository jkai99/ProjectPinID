if (TARGET tinyusb_device)
    add_executable(UART_debugger 
    UART_debugger.c
            )

    # pull in common dependencies
    target_link_libraries(UART_debugger pico_stdlib hardware_i2c hardware_spi hardware_uart)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(UART_debugger 1)
    pico_enable_stdio_uart(UART_debugger 1) #changed from 0 to 1

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(UART_debugger)

    # add url via pico_set_program_url
    example_auto_set_url(UART_debugger)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

if (TARGET tinyusb_device)
    add_executable(oled_test 
    oled_test.c
            )

    # pull in common dependencies
    target_link_libraries(oled_test pico_stdlib hardware_i2c hardware_spi hardware_uart)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(oled_test 1)
    pico_enable_stdio_uart(oled_test 1) #changed from 0 to 1

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(oled_test)

    # add url via pico_set_program_url
    example_auto_set_url(oled_test)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

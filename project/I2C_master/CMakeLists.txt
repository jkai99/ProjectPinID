if (TARGET tinyusb_device)
    add_executable(I2C_master 
    I2C_master.c
            )

    # pull in common dependencies
    target_link_libraries(I2C_master pico_stdlib hardware_i2c hardware_spi hardware_uart)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(I2C_master 1)
    # pico_enable_stdio_uart(I2C_master 1) #changed from 0 to 1

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(I2C_master)

    # add url via pico_set_program_url
    example_auto_set_url(I2C_master)
    
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

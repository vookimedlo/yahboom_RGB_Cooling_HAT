CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
PROJECT(cooling_hat)

MESSAGE("-- RGB Cooling Hat [yahboom] - https://github.com/vookimedlo/yahboom_RGB_Cooling_HAT")

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_C_FLAGS "-Wall -Wextra ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS_RELEASE "-O3 ${CMAKE_C_FLAGS}")

SET(SOURCES
        cooling_hat_control.c
        cooling_hat_fan.c
        cooling_hat_i2c.c
        cooling_hat_information.c
        cooling_hat_oled.c
        cooling_hat_rgb.c
        ssd1306_i2c.c)

ADD_EXECUTABLE(cooling_hat
        ${SOURCES})

TARGET_LINK_LIBRARIES(cooling_hat wiringPi)

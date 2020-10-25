#  BSD 3-Clause License
#
#  Copyright (c) 2020, Michal Duda
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice, this
#     list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#
#  3. Neither the name of the copyright holder nor the names of its
#     contributors may be used to endorse or promote products derived from
#     this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
PROJECT(cooling_hat)

MESSAGE("-- RGB Cooling Hat [yahboom] - https://github.com/vookimedlo/yahboom_RGB_Cooling_HAT")

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_C_FLAGS "-Wall -Wextra -I 3rd_party_code ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS_RELEASE "-O3 ${CMAKE_C_FLAGS}")

SET(SOURCES
        cooling_hat_arguments.c
        cooling_hat_control.c
        cooling_hat_daemon.c
        cooling_hat_fan.c
        cooling_hat_i2c.c
        cooling_hat_information.c
        cooling_hat_oled.c
        cooling_hat_rgb.c
        cooling_hat_utils.c
        3rd_party_code/ssd1306_i2c.c cooling_hat_rgb_effect.c cooling_hat_rgb_effect.h)

ADD_EXECUTABLE(cooling_hat
        ${SOURCES} cooling_hat_utils.c)

TARGET_LINK_LIBRARIES(cooling_hat wiringPi)

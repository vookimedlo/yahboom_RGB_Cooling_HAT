/*
BSD 3-Clause License

Copyright (c) 2020, Michal Duda
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef COOLING_HAT_INFORMATION_H
#define COOLING_HAT_INFORMATION_H

#include <stddef.h>
#include "cooling_hat_fan.h"

/// Provides the information about the current average load that shall be displayed on the OLED panel.
/// @param output_value The memory that shall store the information.
/// @param output_value_size The size of the @see output_value.
/// @remark 1 minute average interval is used for this operation.
void get_average_load(char *output_value, size_t output_value_size);

/// Provides the information about the current RAM usage that shall be displayed on the OLED panel.
/// @param output_value The memory that shall store the information.
/// @param output_value_size The size of the @see output_value.
void get_ram_usage(char *output_value, size_t output_value_size);

/// Provides the information about the disk usage that shall be displayed on the OLED panel.
/// @param output_value The memory that shall store the information.
/// @param output_value_size The size of the @see output_value.
/// @remark The mounted '/' is used for this operation.
void get_disk_usage(char *output_value, size_t output_value_size);

/// Provides the information about the current IPv4 that shall be displayed on the OLED panel.
/// @param output_value The memory that shall store the information.
/// @param output_value_size The size of the @see output_value.
void get_ip_address(char *output_value, size_t output_value_size);

/// Provides the information about the current fan speed that shall be displayed on the OLED panel.
/// @param output_value The memory that shall store the information.
/// @param output_value_size The size of the @see output_value.
void get_fan_speed(char *output_value, size_t output_value_size, enum fan_speed speed);

/// Provides the information that shall be displayed on the OLED panel.
/// @param output_value The memory that shall store the information.
/// @param output_value_size The size of the @see output_value.
/// @param temperature The temperature that shall be shown on the OLED panel.
void get_temperature(char *output_value, size_t output_value_size, double temperature);

/// Provides the CPU temperature.
/// @returns The CPU temperature.
double get_temperature_double();

#endif //COOLING_HAT_INFORMATION_H

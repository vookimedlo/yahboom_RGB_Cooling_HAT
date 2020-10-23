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

#ifndef COOLING_HAT_OLED_H
#define COOLING_HAT_OLED_H

/// Information that shall be displayed on the OLED panel.
struct oled_information {
    char m_average_load[15];     ///< The average load - 1 minute range.
    char m_cpu_temperature[15];  ///< The CPU temperature in Celsius.
    char m_ram_usage[15];        ///< The RAM usage in percents.
    char m_disk_usage[15];       ///< The disk usage in percents -  the mounted '/'.
    char m_network_address[30];  ///< The network IPv4.
    char m_fan_speed[15];        ///< The fan speed in percents.
};

/// Initializes the OLED panel.
void oled_initialization();

/// Clears the whole OLED panel.
void oled_clear();

/// Refreshes OLED panel with the given information.
/// @param values The information for displaying.
void oled_refresh(const struct oled_information *values);

#endif //COOLING_HAT_OLED_H

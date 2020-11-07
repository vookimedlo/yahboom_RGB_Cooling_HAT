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

#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "cooling_hat_arguments.h"
#include "cooling_hat_daemon.h"
#include "cooling_hat_fan.h"
#include "cooling_hat_information.h"
#include "cooling_hat_oled.h"
#include "cooling_hat_rgb.h"
#include "cooling_hat_utils.h"

#define BASIC_DELAY_IN_SECONDS 1
#define DISK_REFRESH_DELAY_IN_LOOP_COUNT 600 // a hour
#define CPU_REFRESH_DELAY_IN_LOOP_COUNT 60 // a minute
#define RAM_REFRESH_DELAY_IN_LOOP_COUNT 60 // a minute
#define TEMP_REFRESH_DELAY_IN_LOOP_COUNT 8 // 8 seconds

static struct oled_information information = {0};
static unsigned int loop_counter = 0;
static double previous_temperature = 0, temperature = 0;
static bool has_information_changed = false;

volatile bool shall_run = true;

void sigterm_handler() {
    shall_run = false;
};

int main(int argc, char *argv[]) {
    handle_arguments(argc, argv);
    signal(SIGTERM, sigterm_handler);

    DEBUG_PRINT("[APP] Initialization");
    rgb_off();
    oled_initialization();
    set_fan_speed(fan_speed_0_percent);
    sleep(BASIC_DELAY_IN_SECONDS);
    DEBUG_PRINT("[APP] Initialization is over");

    // The IPv4 address will be retrieved only once.
    get_ip_address(information.m_network_address, sizeof(information.m_network_address));

    while (shall_run) {
        if (loop_counter % CPU_REFRESH_DELAY_IN_LOOP_COUNT == 0) {
            get_average_load(information.m_average_load, sizeof(information.m_average_load));
            DEBUG_PRINT("[APP] CPU load string `%s`", information.m_average_load);
            has_information_changed = true;
        }
        if (loop_counter % DISK_REFRESH_DELAY_IN_LOOP_COUNT == 0) {
            get_disk_usage(information.m_disk_usage, sizeof(information.m_disk_usage));
            DEBUG_PRINT("[APP] Disk usage string `%s`", information.m_disk_usage);
            has_information_changed = true;
        }
        if (loop_counter % RAM_REFRESH_DELAY_IN_LOOP_COUNT == 0) {
            get_ram_usage(information.m_ram_usage, sizeof(information.m_ram_usage));
            DEBUG_PRINT("[APP] RAM usage string `%s`", information.m_ram_usage);
            has_information_changed = true;
        }
        if (loop_counter % TEMP_REFRESH_DELAY_IN_LOOP_COUNT == 0) {
            temperature = get_temperature_double();
            get_temperature(information.m_cpu_temperature, sizeof(information.m_cpu_temperature), temperature);
            DEBUG_PRINT("[APP] Temperature string `%s`", information.m_cpu_temperature);
        }

        if (previous_temperature != temperature) {
            size_t index = 0;
            has_information_changed = true;
            for (; index < sizeof(temperature_fan_ranges); ++index) {
                if ((uint8_t) (temperature + 0.5) < temperature_fan_ranges[index].temperature) {
                    PRINT("[APP] Temperature threshold found %uC, fan speed %u",
                          temperature_fan_ranges[index].temperature,
                          temperature_fan_ranges[index].speed);
                    set_fan_speed(temperature_fan_ranges[index].speed);
                    get_fan_speed(information.m_fan_speed, sizeof(information.m_fan_speed),
                                  temperature_fan_ranges[index].speed);
                    DEBUG_PRINT("[APP] Fan string `%s`", information.m_fan_speed);
                    break;
                }
            }

            previous_temperature = temperature;
        }

        if (has_information_changed) {
            oled_refresh(&information);
            has_information_changed = false;
        }

        ++loop_counter;
        sleep(BASIC_DELAY_IN_SECONDS);
    }

    PRINT("[APP] Terminating ...");
    oled_clear();
    set_fan_speed(fan_speed_0_percent);
    rgb_off();
    PRINT("[APP] Terminated");
    return 0;
}

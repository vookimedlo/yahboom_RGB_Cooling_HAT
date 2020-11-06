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

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include "cooling_hat_arguments.h"
#include "cooling_hat_daemon.h"
#include "cooling_hat_fan.h"
#include "cooling_hat_i2c.h"
#include "cooling_hat_rgb.h"
#include "cooling_hat_rgb_effect.h"
#include "cooling_hat_utils.h"

struct temperature_fan_range temperature_fan_ranges[10] = {
        {.temperature = 45, .speed = fan_speed_0_percent},
        {.temperature = 50, .speed = fan_speed_40_percent},
        {.temperature = 60, .speed = fan_speed_60_percent},
        {.temperature = 70, .speed = fan_speed_80_percent},
        {.temperature = 255, .speed = fan_speed_100_percent},
        {.temperature = 255, .speed = fan_speed_100_percent}, // Empty positions starts here
        {.temperature = 255, .speed = fan_speed_100_percent},
        {.temperature = 255, .speed = fan_speed_100_percent},
        {.temperature = 255, .speed = fan_speed_100_percent},
        {.temperature = 255, .speed = fan_speed_100_percent},
};

static bool is_numeric(char x) {
    return (x >= '0' && x <= '9') ? true : false;
}

static bool atoi_ex(const char *str, unsigned int *output_value) {
    if (str == NULL || *str == '\0')
        return false;

    *output_value = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (is_numeric(str[i]) == false)
            return false;

        *output_value = *output_value * 10 + str[i] - '0';
    }

    return true;
}

static void parse_fan_ranges(const char *ranges_argument) {
    char ranges_argument_tmp_buffer[255];
    unsigned int ranges_argument_tmp_buffer_length = strlen(ranges_argument);
    char *ranges_argument_token;

    if (!ranges_argument || !ranges_argument_tmp_buffer_length) {
        PRINT("[APP] Wrong format of the fan ranges: no data\n");
        exit(EXIT_FAILURE);
    }

    // Create the own copy to prevent the modification of the input string during the tokenization
    strncpy(ranges_argument_tmp_buffer, ranges_argument, sizeof(ranges_argument_tmp_buffer));

    // Tokenize the first range
    ranges_argument_token = strtok(ranges_argument_tmp_buffer, ":");

    for (size_t i = 0; i < sizeof(temperature_fan_ranges) / sizeof(temperature_fan_ranges[0]); ++i) {
        char *token;
        unsigned int numeric_value;
        char ranges_tmp_buffer[255];

        // Check if the token is not an empty string, e.g. "20,3:30,4:" - the last colon will result in to this.
        if (!ranges_argument_token || *ranges_argument_token == '\0')
            break;

        // Create the own copy to prevent the modification of the input range during the tokenization
        strncpy(ranges_tmp_buffer, ranges_argument_token, sizeof(ranges_tmp_buffer));

        // Tokenize the temperature from the given range
        token = strtok(ranges_tmp_buffer, ",");
        if (!atoi_ex(token, &numeric_value) || (numeric_value > 255)) {
            PRINT("[APP] Wrong format of the temperature: %s\n", token);
            exit(EXIT_FAILURE);
        }
        temperature_fan_ranges[i].temperature = numeric_value;

        // Tokenize the fan speed from the given range
        token = strtok(0, ",");
        if (!atoi_ex(token, &numeric_value) || (numeric_value > 9)) {
            PRINT("[APP] Wrong format of the fan speed: %s\n", token);
            exit(EXIT_FAILURE);
        }
        temperature_fan_ranges[i].speed = numeric_value;

#if 0
        DEBUG_PRINT("[APP] New fan range #%ld - temperature %uC, speed %u",
                    i,
                    temperature_fan_ranges[i].temperature,
                    temperature_fan_ranges[i].speed);
#endif

        // Check if all data were parsed
        if ((ranges_argument_tmp_buffer + ranges_argument_tmp_buffer_length) ==
            (ranges_argument_token + strlen(ranges_argument_token))) {
            // fill the remaining table with highest possible temperature
            for (++i; i < sizeof(temperature_fan_ranges) / sizeof(temperature_fan_ranges[0]); ++i) {
                temperature_fan_ranges[i].temperature = 255;
                temperature_fan_ranges[i].speed = fan_speed_100_percent;
#if 0
                DEBUG_PRINT("[APP] New fan range #%ld - temperature %uC, speed %u",
                            i,
                            temperature_fan_ranges[i].temperature,
                            temperature_fan_ranges[i].speed);
#endif
            }
            return;
        }

        // Tokenize the next range
        ranges_argument_token = strtok(ranges_argument_token + strlen(ranges_argument_token) + 1, ":");
    }

    PRINT("[APP] Wrong format of the fan ranges: %s\n", optarg);
    exit(EXIT_FAILURE);
}

static void show_usage(const char *name) {
    PRINT("%s [-d] [-r fan_range:...] | [-h] | [-f fan_settings] | [-l led_settings] | [-e effect_settings]", name);
    PRINT("\n\tThe following options are available:");
    PRINT("\t\t-h                  Shows usage.");
    PRINT("\t\t-d                  Runs as a daemon.");
    PRINT("\t\t-f fan_settings     Sets the fan speed to the specified value and quits.");
    PRINT("\t\t                    fan_settings:");
    PRINT("\t\t                       0: Fan Off");
    PRINT("\t\t                       1: 100%% speed");
    PRINT("\t\t                       2:  20%% speed");
    PRINT("\t\t                       3:  30%% speed");
    PRINT("\t\t                       4:  40%% speed");
    PRINT("\t\t                       5:  50%% speed");
    PRINT("\t\t                       6:  60%% speed");
    PRINT("\t\t                       7:  70%% speed");
    PRINT("\t\t                       8:  80%% speed");
    PRINT("\t\t                       9:  90%% speed");
    PRINT("\t\t-l led_settings     Sets the given LED to the specified R,G,B values and quits.");
    PRINT("\t\t                    led_settings: LED_NUMBER,RED,GREEN,BLUE");
    PRINT("\t\t                       LED_NUMBER: 0-2");
    PRINT("\t\t                       LED_NUMBER: 3 ... all LEDs");
    PRINT("\t\t                              RED: 0-255");
    PRINT("\t\t                            GREEN: 0-255");
    PRINT("\t\t                             BLUE: 0-255");
    PRINT("\t\t-e effect_settings  Sets the LEDs effect to the specified values and quits.");
    PRINT("\t\t                    effect_settings: EFFECT,SPEED,COLOR");
    PRINT("\t\t                       EFFECT: 0 ... water light");
    PRINT("\t\t                       EFFECT: 1 ... breathing light");
    PRINT("\t\t                       EFFECT: 2 ... marquee");
    PRINT("\t\t                       EFFECT: 3 ... rainbow lights");
    PRINT("\t\t                       EFFECT: 4 ... colorful lights");
    PRINT("\t\t                        SPEED: 0 ... low");
    PRINT("\t\t                        SPEED: 1 ... medium");
    PRINT("\t\t                        SPEED: 2 ... high");
    PRINT("\t\t                        COLOR: 0 ... red");
    PRINT("\t\t                        COLOR: 1 ... green");
    PRINT("\t\t                        COLOR: 2 ... blue");
    PRINT("\t\t                        COLOR: 3 ... yellow");
    PRINT("\t\t                        COLOR: 4 ... purple");
    PRINT("\t\t                        COLOR: 5 ... cyan");
    PRINT("\t\t                        COLOR: 6 ... white");
    PRINT("\t\t-r fan_range        Sets the fan ranges. If multiple fan_range are specified they need to be ordered from the lowest temperature!");
    PRINT("\t\t                    fan_range: UPPER_RANGE,fan_settings");
    PRINT("\t\t                       UPPER_RANGE: 0-255");
}

void handle_arguments(int argc, char *argv[]) {
    int opt;
    bool is_daemon = false;
    bool is_fan_only = false;
    unsigned int fan_only_speed = 0;
    bool is_rgb_only = false;
    unsigned int rgb_number = 0, rgb_red = 0, rgb_green = 0, rgb_blue = 0;
    bool is_effect_only = false;
    unsigned int  effect = 0, effect_speed = 0, effect_color = 0;
    bool is_fan_range_customized = false;

    char tmp_buffer[255];
    char *token;

    while ((opt = getopt(argc, argv, ":l:r:f:hd")) != -1) {
        switch (opt) {
            case 'd':
                is_daemon = true;
                PRINT("[APP] Will run as a daemon");
                break;
            case 'l':
                is_rgb_only = true;
                PRINT("[APP] LED will be set to %s", optarg);
                strncpy(tmp_buffer, optarg, sizeof(tmp_buffer));
                token = strtok(tmp_buffer, ",");
                if (!atoi_ex(token, &rgb_number) || (rgb_number > 3)) {
                    PRINT("[APP] Wrong format of the LED number: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                token = strtok(0, ",");
                if (!atoi_ex(token, &rgb_red) || (rgb_red > 255)) {
                    PRINT("[APP] Wrong format of the LED red value: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                token = strtok(0, ",");
                if (!atoi_ex(token, &rgb_green) || (rgb_green > 255)) {
                    PRINT("[APP] Wrong format of the LED green value: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                token = strtok(0, ",");
                if (!atoi_ex(token, &rgb_blue) || (rgb_blue > 255)) {
                    PRINT("[APP] Wrong format of the LED blue value: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                if (strtok(0, ",")) {
                    PRINT("[APP] Wrong format of the LED value: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'e':
                is_effect_only = true;
                PRINT("[APP] LED effect will be set to %s", optarg);
                strncpy(tmp_buffer, optarg, sizeof(tmp_buffer));
                token = strtok(tmp_buffer, ",");
                if (!atoi_ex(token, &effect) || (effect > 4)) {
                    PRINT("[APP] Wrong format of the effect: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                token = strtok(0, ",");
                if (!atoi_ex(token, &effect_speed) || (effect_speed > 3)) {
                    PRINT("[APP] Wrong format of the effect speed: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                token = strtok(0, ",");
                if (!atoi_ex(token, &effect_color) || (effect_color > 6)) {
                    PRINT("[APP] Wrong format of the effect color: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                if (strtok(0, ",")) {
                    PRINT("[APP] Wrong format of the effect value: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'f':
                PRINT("[APP] Fan will be set to: %s\n", optarg);
                if (!atoi_ex(optarg, &fan_only_speed) || (fan_only_speed > 9)) {
                    PRINT("[APP] Wrong format of the fan speed: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                is_fan_only = true;
                break;
            case 'r':
                PRINT("[APP] Fan ranges will be set to: : %s\n", optarg);
                parse_fan_ranges(optarg);
                is_fan_range_customized = true;
                break;
            case 'h':
                show_usage(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case ':':
                PRINT("[APP] Option needs a value");
                exit(EXIT_FAILURE);
                break;
            case '?':
                PRINT("[APP] Unknown option: %c", optopt);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (optind < argc) {
        for (; optind < argc; ++optind) {
            printf("Unknown argument: %s\n", argv[optind]);
        }
        exit(EXIT_FAILURE);
    }

    if ((is_effect_only && is_rgb_only) ||
        (is_rgb_only && is_fan_only) ||
        (is_effect_only && is_fan_only) ||
        (is_rgb_only | is_effect_only | is_fan_only && is_fan_range_customized | is_daemon)) {
        PRINT("[APP] Incompatible options\n\n");
        exit(EXIT_FAILURE);
    }

    if (!i2c_init()) {
        PRINT("[APP] I2C initialization failure");
        exit(EXIT_FAILURE);
    }

    if (is_effect_only) {
        set_rgb_effect(effect, effect_speed, effect_color);
        DEBUG_PRINT("[APP] Terminating ...");
        exit(EXIT_SUCCESS);
    }

    if (is_rgb_only) {
        if (rgb_red | rgb_green | rgb_blue)
            set_rgb(rgb_number, rgb_red, rgb_green, rgb_blue);
        else
            rgb_off();
        DEBUG_PRINT("[APP] Terminating ...");
        exit(EXIT_SUCCESS);
    }

    if (is_fan_only) {
        set_fan_speed(fan_only_speed);
        DEBUG_PRINT("[APP] Terminating ...");
        exit(EXIT_SUCCESS);
    }

    for (size_t i = 0; i < sizeof(temperature_fan_ranges) / sizeof(temperature_fan_ranges[0]); ++i)
        PRINT("[APP] Fan range #%ld - temperature %uC, speed %u",
              i,
              temperature_fan_ranges[i].temperature,
              temperature_fan_ranges[i].speed);

    if (is_daemon)
        daemonize();
}

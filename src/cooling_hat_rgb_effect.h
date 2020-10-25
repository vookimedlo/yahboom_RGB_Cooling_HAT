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

#ifndef COOLING_HAT_RGB_EFFECT_H
#define COOLING_HAT_RGB_EFFECT_H

#include <stdint.h>

/// LEDs.
enum rgb_effect {
    rgb_effect_water_light = 0,
    rgb_effect_breathing_light = 1,
    rgb_effect_marqueee = 2,
    rgb_effect_rainbow_lights = 3,
    rgb_effect_colorful_lights = 4,
};

enum rgb_effect_speed {
    rgb_effect_speed_low = 1,
    rgb_effect_speed_medium = 2,
    rgb_effect_speed_high = 3,
};

enum rgb_effect_color {
    rgb_effect_color_red = 0,
    rgb_effect_color_green = 1,
    rgb_effect_color_blue = 2,
    rgb_effect_color_yellow = 3,
    rgb_effect_color_purple = 4,
    rgb_effect_color_cyan = 5,
    rgb_effect_color_white = 6,
};

/// Sets the LED effect.
/// @param effect The intended LED that shall be changed.
/// @param speed The red value.
/// @param color The green value.
void set_rgb_effect(enum rgb_effect effect, enum rgb_effect_speed speed, enum rgb_effect_color color);

#endif //COOLING_HAT_RGB_EFFECT_H

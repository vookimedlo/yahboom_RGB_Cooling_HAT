#ifndef COOLING_HAT_RGB_H
#define COOLING_HAT_RGB_H

enum rgb_light {
    rgb_1_light = 0,
    rgb_2_light = 1,
    rgb_3_light = 2,
    rgb_all_lights = 3,
};

void set_rgb(uint8_t value);

void rgb_off();

#endif //COOLING_HAT_RGB_H

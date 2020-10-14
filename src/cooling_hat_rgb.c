#include "cooling_hat_rgb.h"
#include "cooling_hat_i2c.h"

void set_rgb(enum rgb_light light, uint8_t value) {
    cooling_i2c_write(i2c_rgb_light_register, light);
    cooling_i2c_write(i2c_red_register, value);
    cooling_i2c_write(i2c_green_register, value);
    cooling_i2c_write(i2c_blue_register, value);
}

void rgb_off() {
    cooling_i2c_write(i2c_rgb_register, 0);
}

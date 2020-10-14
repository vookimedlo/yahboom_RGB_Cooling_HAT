#include "cooling_hat_rgb.h"
#include "cooling_hat_i2c.h"

void set_red_led(uint8_t value) {
    cooling_i2c_write(i2c_red_register, value);
}

void set_green_led(uint8_t value) {
    cooling_i2c_write(i2c_green_register, value);
}

void set_blue_led(uint8_t value) {
    cooling_i2c_write(i2c_blue_register, value);
}

void set_rgb(uint8_t value) {
    cooling_i2c_write(i2c_rgb_register, value);
}

void rgb_off() {
    cooling_i2c_write(i2c_rgb_register, 0);
}

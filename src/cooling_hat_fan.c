#include "cooling_hat_fan.h"
#include "cooling_hat_i2c.h"

void set_fan_speed(enum fan_speed value) {
    i2c_write(i2c_fan_register, value);
}

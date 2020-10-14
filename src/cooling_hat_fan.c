#include "cooling_hat_fan.h"
#include "cooling_hat_i2c.h"

void set_fan_speed(enum fan_speed value) {
    cooling_i2c_write(fan_address, value);
}

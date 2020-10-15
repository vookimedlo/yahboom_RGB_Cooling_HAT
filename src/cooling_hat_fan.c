#include "cooling_hat_fan.h"
#include "cooling_hat_i2c.h"
#include "cooling_hat_utils.h"

static enum fan_speed last_fan_speed = fan_speed_0_percent;

void set_fan_speed(enum fan_speed value) {
    if (last_fan_speed == value)
        return;

    last_fan_speed = value;

    DEBUG_PRINT("[I2C] Changing fan speed to %d", value);
    i2c_write(i2c_fan_register, value);
}

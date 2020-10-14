#include "cooling_hat_fan.h"
#include "cooling_hat_i2c.h"
#include "cooling_hat_information.h"
#include "cooling_hat_oled.h"
#include "cooling_hat_rgb.h"

int main(char *argv[], int argv) {

    if (!i2c_init()) {
        return -1;
    }

    oled_initialization();
    set_fan_speed(fan_speed_0_percent);
    rgb_off();


    return 0;
}

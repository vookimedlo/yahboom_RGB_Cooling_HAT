#include "cooling_hat_i2c.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

static int fd_i2c = -1;

bool i2c_init() {
    fd_i2c = wiringPiI2CSetup(0x0d);
    return fd_i2c >= 0 ? true : false;
}

void i2c_write(enum i2c_register reg, uint8_t data) {
    wiringPiI2CWriteReg8(fd_i2c, address, data);
}
#include "app_main.h"

void app_i2c_init() {
    drv_i2c_gpio_set(I2C_PIN_GROUP);
    drv_i2c_master_init(I2C_CLOCK);
}

int8_t scan_i2c_addr(uint8_t address) {
    if ((reg_clk_en0 & FLD_CLK0_I2C_EN)==0)
        app_i2c_init();
    u8 r = reg_i2c_speed;
    reg_i2c_speed = (u8)(CLOCK_SYS_CLOCK_HZ/(4*100000)); // 100 kHz
    reg_i2c_id = (u8) address;
    reg_i2c_ctrl = FLD_I2C_CMD_START | FLD_I2C_CMD_ID | FLD_I2C_CMD_STOP;
    while (reg_i2c_status & FLD_I2C_CMD_BUSY);
    reg_i2c_speed = r;
    return ((reg_i2c_status & FLD_I2C_NAK)? 0 : address);
}


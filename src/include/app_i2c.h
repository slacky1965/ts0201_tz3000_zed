#ifndef SRC_INCLUDE_APP_I2C_H_
#define SRC_INCLUDE_APP_I2C_H_

void app_i2c_init();

#if (I2C_DRV_USED == I2C_DRV_HARD)
int8_t scan_i2c_addr(uint8_t address);
#elif (I2C_DRV_USED == I2C_DRV_SOST)
unsigned char scan_i2c_addr(unsigned char address);
#endif

#endif /* SRC_INCLUDE_APP_I2C_H_ */

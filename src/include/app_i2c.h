#ifndef SRC_INCLUDE_APP_I2C_H_
#define SRC_INCLUDE_APP_I2C_H_

void app_i2c_init();

#if (I2C_DRV_USED == I2C_DRV_HARD)
uint8_t scan_i2c_addr(uint8_t address);
#elif (I2C_DRV_USED == I2C_DRV_SOFT)
unsigned char scan_i2c_addr(unsigned char address);
int8_t read_i2c_bytes(unsigned char i2c_addr, unsigned char * dataBuf, int dataLen);
int8_t send_i2c_bytes(unsigned char i2c_addr, unsigned char * dataBuf, size_t dataLen);
#endif

#endif /* SRC_INCLUDE_APP_I2C_H_ */

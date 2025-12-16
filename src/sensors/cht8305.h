#ifndef SRC_SENSORS_CHT8305_H_
#define SRC_SENSORS_CHT8305_H_

#if (SENSOR_USED == SENSOR_CHT8305)

//#include "app_main.h"

#define CHT8305_I2C_ADDRESS         0x40
#define CHT8305_I2C_ADDRESS2        0x41
#define CHT8305_I2C_ADDRESS3        0x42
#define CHT8305_I2C_ADDRESS4        0x43

#define CHT8305_REG_TMP             0x00
#define CHT8305_REG_HMD             0x01
#define CHT8305_REG_CFG             0x02
#define CHT8305_REG_ALR             0x03
#define CHT8305_REG_VLT             0x04
#define CHT8305_REG_MID             0xfe
#define CHT8305_REG_VID             0xff

//  Config register mask
#define CHT8305_CFG_SOFT_RESET      0x8000
#define CHT8305_CFG_CLOCK_STRETCH   0x4000
#define CHT8305_CFG_HEATER          0x2000
#define CHT8305_CFG_MODE            0x1000
#define CHT8305_CFG_VCCS            0x0800
#define CHT8305_CFG_TEMP_RES        0x0400
#define CHT8305_CFG_HUMI_RES        0x0300
#define CHT8305_CFG_ALERT_MODE      0x00C0
#define CHT8305_CFG_ALERT_PENDING   0x0020
#define CHT8305_CFG_ALERT_HUMI      0x0010
#define CHT8305_CFG_ALERT_TEMP      0x0008
#define CHT8305_CFG_VCC_ENABLE      0x0004
#define CHT8305_CFG_VCC_RESERVED    0x0003

#define CHT8305_MID 0x5959
#define CHT8305_VID 0x8305

#define CHT8305_DELAY_CFG           5           // 5 ms
#define CHT8305_DELAY_MEASURING     13          // 13 ms

typedef enum {
    CHT8305_OK = 0,
    CHT8305_ERROR,
    CHT8305_ERR_NULL_PTR,
    CHT8305_ERR_INIT_FAIL,
    CHT8305_ERR_COMM_FAIL,
    CHT8305_ERR_MODE_FAIL,
    CHT8305_ERR_INVALID_LEN,
    CHT8305_ERR_DEV_NOT_FOUND,
    CHT8305_ERR_ADDR_NOT_FOUND,
    CHT8305_ERR_CRC,
    CHT8305_ERR_ID,
} cht8305_error_t;

struct _cht8305_dev;

typedef struct _cht8305_dev cht8305_dev_t;

typedef int8_t (*cht8305_read_t)(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, cht8305_dev_t *dev);
typedef int8_t (*cht8305_write_t)(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, cht8305_dev_t *dev);
typedef void   (*cht8305_delay_t)(uint32_t period);

struct _cht8305_dev {
    uint8_t         addr;
    cht8305_read_t  read;
    cht8305_write_t write;
    cht8305_delay_t delay;
    uint32_t        id;
    uint16_t         raw_temp;
    uint16_t        raw_hum;
};

cht8305_error_t cht8305_init(cht8305_dev_t *dev);
cht8305_error_t cht8305_readSensor();

#endif

#endif /* SRC_SENSORS_CHT8305_H_ */

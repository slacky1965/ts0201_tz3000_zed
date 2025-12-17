#ifndef SRC_SENSORS_SHT40_H_
#define SRC_SENSORS_SHT40_H_

#if (SENSOR_USED == SENSOR_SHT40)

#define SHT4X_I2C_ADDRESS                                   0x44
#define SHT4X_I2C_ADDRESS2                                  0x45
#define SHT4X_I2C_ADDRESS3                                  0x46

#define SHT4X_HIGH_PRECISION_WITH_NO_HEATER                 0xFD
#define SHT4X_MEDIUM_PRECISION_WITH_NO_HEATER               0xF6
#define SHT4X_LOWEST_PRECISION_WITH_NO_HEATER               0xE0
#define SHT4X_HIGH_PRECISION_WITH_HEATER_200MW_1S           0x39
#define SHT4X_HIGH_PRECISION_WITH_HEATER_200MW_0P1S         0x32
#define SHT4X_HIGH_PRECISION_WITH_HEATER_110MW_1S           0x2F
#define SHT4X_HIGH_PRECISION_WITH_HEATER_110MW_0P1S         0x24
#define SHT4X_HIGH_PRECISION_WITH_HEATER_20MW_1S            0x1E
#define SHT4X_HIGH_PRECISION_WITH_HEATER_20MW_0P1S          0x15

#define SHT4X_SOFT_RESET                                    0x94
#define SHT4X_READ_SERIAL_NUMBER                            0x89

#define SHT4X_DELAY_SOFT_RESET                              10
#define SHT4X_DELAY_MEASURING                               10

typedef enum {
    SHT40_OK = 0,
    SHT40_ERROR,
    SHT40_ERR_NULL_PTR,
    SHT40_ERR_INIT_FAIL,
    SHT40_ERR_COMM_FAIL,
    SHT40_ERR_MODE_FAIL,
    SHT40_ERR_INVALID_LEN,
    SHT40_ERR_DEV_NOT_FOUND,
    SHT40_ERR_ADDR_NOT_FOUND,
    SHT40_ERR_CRC,
    SHT40_ERR_ID,
} sht40_error_t;

struct _sht40_dev;

typedef struct _sht40_dev sht40_dev_t;

typedef int8_t (*sht40_read_t)(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, sht40_dev_t *dev);
typedef int8_t (*sht40_write_t)(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, sht40_dev_t *dev);
typedef void   (*sht40_delay_t)(uint32_t period);
//typedef int8_t (*sht40_read_t)(uint8_t *reg_data, uint32_t len, sht40_dev_t *dev);
//typedef int8_t (*sht40_write_t)(const uint8_t *reg_data, uint32_t len, sht40_dev_t *dev);
//typedef void   (*sht40_delay_t)(uint32_t period);

struct _sht40_dev {
    uint8_t         addr;
    sht40_read_t    read;
    sht40_write_t   write;
    sht40_delay_t   delay;
    uint32_t        id;
    uint16_t        raw_temp;
    uint16_t        raw_hum;
};

sht40_error_t sht40_init(sht40_dev_t *dev);
sht40_error_t sht40_readSensor();


#endif /* SENSOR_USED */

#endif /* SRC_SENSORS_SHT40_H_ */

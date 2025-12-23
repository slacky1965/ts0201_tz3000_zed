#ifndef SRC_SENSORS_AHT20_H_
#define SRC_SENSORS_AHT20_H_

#if (SENSOR_USED == SENSOR_AHT20)

#define AHT20_I2C_ADDRESS               0x38

#define AHT20_SOFT_RESET                0xBA
#define AHT20_REG_1B                    0x1B
#define AHT20_REG_1C                    0x1C
#define AHT20_REG_1E                    0x1E
#define AHT20_MEASUREMENT_TRIGGER       0xAC
#define AHT20_MEASUREMENT_DATA1         0x33
#define AHT20_MEASUREMENT_DATA2         0x00

#define AHT20_DELAY_POWER               250
#define AHT20_DELAY_SOFT_RESET          10
#define AHT20_DELAY_REG_RESET           5
#define AHT20_DELAY_DEFAULT             10
#define AHT20_DELAY_MEASURING           85


typedef enum {
    AHT20_OK = 0,
    AHT20_ERROR,
    AHT20_ERR_NULL_PTR,
    AHT20_ERR_INIT_FAIL,
    AHT20_ERR_COMM_FAIL,
    AHT20_ERR_MODE_FAIL,
    AHT20_ERR_INVALID_LEN,
    AHT20_ERR_DEV_NOT_FOUND,
    AHT20_ERR_ADDR_NOT_FOUND,
    AHT20_ERR_CRC,
    AHT20_ERR_ID,
} aht20_error_t;

struct _aht20_dev;

typedef struct _aht20_dev aht20_dev_t;

typedef int8_t (*aht20_read_t)(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, aht20_dev_t *dev);
typedef int8_t (*aht20_write_t)(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, aht20_dev_t *dev);
typedef void   (*aht20_delay_t)(uint32_t period);

struct _aht20_dev {
    uint8_t         addr;
    aht20_read_t    read;
    aht20_write_t   write;
    aht20_delay_t   delay;
    uint32_t        id;
    uint32_t        raw_temp;
    uint32_t        raw_hum;
};

aht20_error_t aht20_init(aht20_dev_t *dev);
aht20_error_t aht20_readSensor();

#endif /* (SENSOR_USED == SENSOR_AHT20) */

#endif /* SRC_SENSORS_AHT20_H_ */

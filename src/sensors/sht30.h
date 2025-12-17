#ifndef SRC_SENSORS_SHT30_H_
#define SRC_SENSORS_SHT30_H_

#if (SENSOR_USED == SENSOR_SHT30)

#define SHT30_I2C_ADDRESS                       0x44
#define SHT30_I2C_ADDRESS2                      0x45

#define SHT30_READ_SENSOR                       0x2C06

#define SHT30_FETCH_DATA                        0xE000
#define SHT30_ART                               0x2B32
#define SHT30_BREAK                             0x3093
#define SHT30_SOFT_RESET                        0x30A2
#define SHT30_HEATER_ENABLE                     0x306D
#define SHT30_HEATER_DISABLE                    0x3066
#define SHT30_READ_STATUS                       0xF32D
#define SHT30_CLEAR_STATUS                      0x3041
#define SHT30_GET_SERIAL_NUMBER                 0x3682
#define SHT30_READ_HIGH_ALERT_LIMIT_SET         0xE11F
#define SHT30_READ_HIGH_ALERT_LIMIT_CLEAR       0xE114
#define SHT30_READ_LOW_ALERT_LIMIT_SET          0xE102
#define SHT30_READ_LOW_ALERT_LIMIT_CLEAR        0xE109
#define SHT30_WRITE_HIGH_ALERT_LIMIT_SET        0x611D
#define SHT30_WRITE_HIGH_ALERT_LIMIT_CLEAR      0x6116
#define SHT30_WRITE_LOW_ALERT_LIMIT_SET         0x6100
#define SHT30_WRITE_LOW_ALERT_LIMIT_CLEAR       0x610B

#define SHT30_DELAY_DEFAULT                     10
#define SHT30_DELAY_MEASURING                   30

typedef enum {
    SHT30_REPEATABILITY_HIGH   = 0x00,
    SHT30_REPEATABILITY_MEDIUM,
    SHT30_REPEATABILITY_LOW
} sht30_repeatability_t;


typedef enum {
    SHT30_OK = 0,
    SHT30_ERROR,
    SHT30_ERR_NULL_PTR,
    SHT30_ERR_INIT_FAIL,
    SHT30_ERR_COMM_FAIL,
    SHT30_ERR_MODE_FAIL,
    SHT30_ERR_INVALID_LEN,
    SHT30_ERR_DEV_NOT_FOUND,
    SHT30_ERR_ADDR_NOT_FOUND,
    SHT30_ERR_CRC,
    SHT30_ERR_ID,
} sht30_error_t;

struct _sht30_dev;

typedef struct _sht30_dev sht30_dev_t;

typedef int8_t (*sht30_read_t)(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, sht30_dev_t *dev);
typedef int8_t (*sht30_write_t)(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, sht30_dev_t *dev);
typedef void   (*sht30_delay_t)(uint32_t period);

struct _sht30_dev {
    uint8_t         addr;
    sht30_read_t    read;
    sht30_write_t   write;
    sht30_delay_t   delay;
    uint32_t        id;
    uint16_t        raw_temp;
    uint16_t        raw_hum;
    uint8_t         repeatability;
};

sht30_error_t sht30_init(sht30_dev_t *dev);
sht30_error_t sht30_readSensor();
sht30_error_t sht30_set_art();
sht30_error_t sht30_set_heater(uint8_t enable);

#endif // SENSOR_USED == SENSOR_SHT30

#endif /* SRC_SENSORS_SHT30_H_ */

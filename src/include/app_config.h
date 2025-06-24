#ifndef SRC_INCLUDE_APP_CONFIG_H_
#define SRC_INCLUDE_APP_CONFIG_H_

#define READ_SENSORS_PERIOD_MIN         5
#define READ_SENSORS_PERIOD_MAX         600
#define TEMPERATURE_OFFSET_MIN         -500
#define TEMPERATURE_OFFSET_MAX          500
#define TEMPERATURE_ONOFF_MIN          -4000
#define TEMPERATURE_ONOFF_MAX           12500
#define TEMPERATURE_ONOFF_ENABLED       0x01
#define TEMPERATURE_ONOFF_DISABLED      0x00
#define HUMIDITY_OFFSET_MIN            -1000
#define HUMIDITY_OFFSET_MAX             1000
#define HUMIDITY_ONOFF_MIN              0
#define HUMIDITY_ONOFF_MAX              10000
#define HUMIDITY_ONOFF_ENABLED          0x01
#define HUMIDITY_ONOFF_DISABLED         0x00

#define DEFAULT_READ_SENSORS_PERIOD     (READ_SENSORS_PERIOD_MIN * 2)
#define DEFAULT_TEMPERATURE_OFFSET      0
#define DEFAULT_TEMPERATURE_ONOFF       TEMPERATURE_ONOFF_DISABLED
#define DEFAULT_TEMPERATURE_ONOFF_MIN   TEMPERATURE_ONOFF_MIN
#define DEFAULT_TEMPERATURE_ONOFF_MAX   TEMPERATURE_ONOFF_MAX
#define DEFAULT_HUMIDITY_OFFSET         0
#define DEFAULT_HUMIDITY_ONOFF          HUMIDITY_ONOFF_DISABLED
#define DEFAULT_HUMIDITY_ONOFF_MIN      HUMIDITY_ONOFF_MIN
#define DEFAULT_HUMIDITY_ONOFF_MAX      HUMIDITY_ONOFF_MAX

typedef struct __attribute__((packed)) {
    uint16_t    read_sensors_period;
    int16_t     temperature_offset;     /* -5 ... +5 step 0.1 * 100     */
    uint8_t     temperature_onoff;      /* 0 - disabled, 1 - enabled    */
    uint16_t    temperature_onoff_low;  /* -4000                        */
    uint16_t    temperature_onoff_high; /* 12500                        */
    int16_t     humidity_offset;        /* -10 ... +10 % step 1 * 100   */
    uint8_t     humidity_onoff;         /* 0 - disabled, 1 - enabled    */
    uint16_t    humidity_onoff_low;     /* 0                            */
    uint16_t    humidity_onoff_high;    /* 10000                        */
    uint8_t     crc;
} config_t;

extern config_t config;

nv_sts_t config_save();
nv_sts_t config_restore();

#endif /* SRC_INCLUDE_APP_CONFIG_H_ */

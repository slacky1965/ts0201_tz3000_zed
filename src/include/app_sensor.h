#ifndef SRC_INCLUDE_APP_SENSOR_H_
#define SRC_INCLUDE_APP_SENSOR_H_

#include "../sensors/cht8305.h"
#include "../sensors/sht30.h"

typedef enum {
    SENSOR_OK = 0,
    SENSOR_ERROR,
    SENSOR_ERR_NULL_PTR,
    SENSOR_ERR_INIT_FAIL,
    SENSOR_ERR_COMM_FAIL,
    SENSOR_ERR_MODE_FAIL,
    SENSOR_ERR_INVALID_LEN,
    SENSOR_ERR_DEV_NOT_FOUND,
    SENSOR_ERR_ADDR_NOT_FOUND,
    SENSOR_ERR_CRC,
    SENSOR_ERR_ID,
} sensor_error_t;


uint8_t app_cht8305_init();
void app_cht8305_measurement();
void app_cht8305_set_temperature();
void app_cht8305_set_humidity();

uint8_t app_sht30_init();
void app_sht30_measurement();
void app_sht30_set_temperature();
void app_sht30_set_humidity();

uint8_t app_sensor_init();
void app_sensor_measurement();
void  app_sensor_set_temperature();
void app_sensor_set_humidity();

#endif /* SRC_INCLUDE_APP_SENSOR_H_ */

#ifndef SRC_INCLUDE_APP_SENSOR_H_
#define SRC_INCLUDE_APP_SENSOR_H_

#include "cht8305.h"
#include "sht30.h"
#include "sht40.h"
#include "aht20.h"

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

sensor_error_t app_sensor_init();
void    app_sensor_measurement();
void    app_sensor_set_temperature();
void    app_sensor_set_humidity();
uint16_t app_sensor_get_period();

#endif /* SRC_INCLUDE_APP_SENSOR_H_ */

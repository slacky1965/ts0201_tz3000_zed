#include "tl_common.h"

//
#include "app_cht8305.h"
#include "app_sht30.h"
#include "app_sensor.h"

uint8_t app_sensor_init() {
    uint8_t ret = 0;

#if (SENSOR_USED == SENSOR_CHT8305)
    app_cht8305_init();
#elif (SENSOR_USED == SENSOR_SHT30)
    app_sht30_init();
#else
#error Sensor not defined!
#endif

    return ret;
}

void app_sensor_measurement() {
#if (SENSOR_USED == SENSOR_CHT8305)
    app_cht8305_measurement();
#elif (SENSOR_USED == SENSOR_SHT30)
    app_sht30_measurement();
#else
#error Sensor not defined!
#endif
}

void  app_sensor_set_temperature() {
#if (SENSOR_USED == SENSOR_CHT8305)
    app_cht8305_set_temperature();
#elif (SENSOR_USED == SENSOR_SHT30)
    app_sht30_set_temperature();
#else
#error Sensor not defined!
#endif
}

void app_sensor_set_humidity() {
#if (SENSOR_USED == SENSOR_CHT8305)
    app_cht8305_measurement();
#elif (SENSOR_USED == SENSOR_SHT30)
    app_sht30_measurement();
#else
#error Sensor not defined!
#endif
}

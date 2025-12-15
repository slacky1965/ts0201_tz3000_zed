#ifndef SRC_INCLUDE_APP_SENSOR_H_
#define SRC_INCLUDE_APP_SENSOR_H_

//#include "app_cht8305.h"
//#include "app_sht30.h"

uint8_t app_sensor_init();
void app_sensor_measurement();
void  app_sensor_set_temperature();
void app_sensor_set_humidity();

#endif /* SRC_INCLUDE_APP_SENSOR_H_ */

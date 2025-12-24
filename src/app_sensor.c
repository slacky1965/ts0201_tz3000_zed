#include "app_main.h"

typedef struct {
    uint8_t (*init)();
    void    (*measurement)();
    void    (*set_temperature)();
    void    (*set_humidity)();
    bool    inited;
    int16_t temperature;
    int16_t humidity;
} sensor_t;

static sensor_t sensor;

#if UART_PRINTF_MODE && DEBUG_SENSOR
static void sensor_error_codes_print_result(const char api_name[], int8_t rslt, uint8_t addr) {
    if (rslt != SENSOR_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case SENSOR_ERR_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf("It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case SENSOR_ERR_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf("It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case SENSOR_ERR_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n", rslt);
                break;

            case SENSOR_ERR_ADDR_NOT_FOUND:
                printf("Error [%d] : Device not found at address. Address 0x%02x not correct\r\n", rslt, addr);
                break;

            case SENSOR_ERR_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            case SENSOR_ERR_CRC:
                printf("Error [%d] : Invalid crc\r\n", rslt);
                break;

//            case SENSOR_ERR_NOT_READY:
//                printf("Error [%d] : Device not ready. Output buffer is empty\r\n", rslt);
//                break;

            case SENSOR_ERR_MODE_FAIL:
                printf("Error [%d] : The device cannot accept commands. The device is in periodic measurement mode.\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}
#endif



#if (SENSOR_USED == SENSOR_CHT8305)


static cht8305_dev_t cht8305_dev;

static int8_t cht8305_i2c_read(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, cht8305_dev_t *dev) {
    drv_i2c_read_series(dev->addr << 1, reg_addr, reg_len, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

static int8_t cht8305_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, cht8305_dev_t *dev) {
    drv_i2c_write_series(dev->addr << 1, reg_addr, 1, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

static void cht8305_delay(uint32_t period) {
  sleep_ms(period);
}

static uint8_t app_cht8305_dev_set(uint8_t slaveID) {

    uint8_t addr = slaveID << 1;

    for (uint8_t i = 0; i < 3; i++) {
        if (scan_i2c_addr(addr) == addr) {
            cht8305_dev.addr = slaveID;
            cht8305_dev.delay = cht8305_delay;
            cht8305_dev.read = cht8305_i2c_read;
            cht8305_dev.write = cht8305_i2c_write;
            return SENSOR_OK;
        } else {
#if UART_PRINTF_MODE && DEBUG_SENSOR
            sensor_error_codes_print_result("app_cht8305_dev_set", SENSOR_ERR_ADDR_NOT_FOUND, slaveID);
#endif
        }
    }

    return SENSOR_ERR_ADDR_NOT_FOUND;
}

static uint8_t app_cht8305_init() {

    cht8305_dev.delay = NULL;
    cht8305_dev.read = NULL;
    cht8305_dev.write = NULL;
    cht8305_dev.addr = 0;
    cht8305_dev.id = 0;

    uint8_t ret;

    if (app_cht8305_dev_set(CHT8305_I2C_ADDRESS) != SENSOR_OK) {
        if (app_cht8305_dev_set(CHT8305_I2C_ADDRESS2) != SENSOR_OK) {
            if (app_cht8305_dev_set(CHT8305_I2C_ADDRESS3) != SENSOR_OK) {
                if (app_cht8305_dev_set(CHT8305_I2C_ADDRESS4) != SENSOR_OK) {
                    return SENSOR_ERR_ADDR_NOT_FOUND;
                }
            }
        }
    }

    ret = cht8305_init(&cht8305_dev);

#if UART_PRINTF_MODE && DEBUG_SENSOR
    sensor_error_codes_print_result("cht8305_init", ret, cht8305_dev.addr);
#endif

//    printf("cht8305 ID: 0x%08x\r\n", cht8305_dev.id);

    return ret;
}

static void  app_cht8305_set_temperature() {

    int16_t temperature = ((int32_t)(16500 * cht8305_dev.raw_temp) / 65535) - 4000 + config.temperature_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
        printf("temperature: %d\r\n", temperature);
#endif

        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&temperature);
}

static void app_cht8305_set_humidity() {

    int16_t humidity = ((uint32_t)(10000 * cht8305_dev.raw_hum) / 65535) + config.humidity_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
    printf("humidity: %d\r\n", humidity);
#endif
    if (humidity < 0)
        humidity = 0;
    if (humidity > 10000)
        humidity = 10000;

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&humidity);
}

static void app_cht8305_measurement() {

    app_i2c_init();

    uint8_t ret = cht8305_readSensor();

#if UART_PRINTF_MODE && DEBUG_SENSOR
    sensor_error_codes_print_result("app_cht8305_measurement", ret, cht8305_dev.addr);
//    printf("temperature_raw: 0x%04x\r\n", cht8305_dev.raw_temp);
//    printf("humidity_raw:    0x%04x\r\n", cht8305_dev.raw_hum);
#endif

    if (ret == SENSOR_OK) {
        app_cht8305_set_temperature();
        app_cht8305_set_humidity();
    }
}

#elif (SENSOR_USED == SENSOR_SHT30)

static sht30_dev_t sht30_dev;

static int8_t sht30_i2c_read(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, sht30_dev_t *dev) {
    drv_i2c_read_series(dev->addr << 1, reg_addr, reg_len, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

static int8_t sht30_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, sht30_dev_t *dev) {
    drv_i2c_write_series(dev->addr << 1, reg_addr, 2, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

static void sht30_delay(uint32_t period) {
  sleep_ms(period);
}

static uint8_t app_sht30_dev_set(uint8_t slaveID) {

    uint8_t addr = slaveID << 1;

    for (uint8_t i = 0; i < 3; i++) {
        if (scan_i2c_addr(addr) == addr) {
            sht30_dev.addr = slaveID;
            sht30_dev.delay = sht30_delay;
            sht30_dev.read = sht30_i2c_read;
            sht30_dev.write = sht30_i2c_write;
            sht30_dev.repeatability = SHT30_REPEATABILITY_HIGH;
            return SENSOR_OK;
        } else {
#if UART_PRINTF_MODE && DEBUG_SENSOR
            sensor_error_codes_print_result("app_sht30_dev_set", SENSOR_ERR_ADDR_NOT_FOUND, slaveID);
#endif
        }
    }

    return SENSOR_ERR_ADDR_NOT_FOUND;
}

static uint8_t app_sht30_init() {

    sht30_dev.delay = NULL;
    sht30_dev.read = NULL;
    sht30_dev.write = NULL;
    sht30_dev.addr = 0;
    sht30_dev.id = 0;

    uint8_t ret;

    if (app_sht30_dev_set(SHT30_I2C_ADDRESS) != SENSOR_OK) {
        if (app_sht30_dev_set(SHT30_I2C_ADDRESS2) != SENSOR_OK) {
            return SENSOR_ERR_ADDR_NOT_FOUND;
        }
    }

    ret = sht30_init(&sht30_dev);
#if UART_PRINTF_MODE && DEBUG_SENSOR
    sensor_error_codes_print_result("sht30_init", ret, sht30_dev.addr);
#endif

    if (ret == SENSOR_OK) {
        sht30_dev.delay(SHT30_DELAY_DEFAULT);
        ret = sht30_set_art();

        if (ret == SENSOR_OK) {
            sht30_dev.delay(SHT30_DELAY_DEFAULT);
            ret = sht30_set_heater(OFF);
        }
    }

    return ret;
}

static void  app_sht30_set_temperature() {

    int16_t temperature = ((int32_t)(17500 * sht30_dev.raw_temp) / 65535) - 4500 + config.temperature_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
        printf("temperature: %d\r\n", temperature);
#endif

        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&temperature);
}

static void app_sht30_set_humidity() {

    int16_t humidity = ((uint32_t)(10000 * sht30_dev.raw_hum) / 65535) + config.humidity_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
    printf("humidity: %d\r\n", humidity);
#endif
    if (humidity < 0)
        humidity = 0;
    if (humidity > 10000)
        humidity = 10000;

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&humidity);
}

static void app_sht30_measurement() {

    app_i2c_init();

    uint8_t ret = sht30_readSensor();

#if UART_PRINTF_MODE && DEBUG_SENSOR
    sensor_error_codes_print_result("app_sht30_measurement", ret, sht30_dev.addr);
//    printf("temperature_raw: 0x%04x\r\n", sht30_dev.raw_temp);
//    printf("humidity_raw:    0x%04x\r\n", sht30_dev.raw_hum);
#endif

    if (ret == SENSOR_OK) {
        app_sht30_set_temperature();
        app_sht30_set_humidity();
    }
}

#elif (SENSOR_USED == SENSOR_SHT40)

static sht40_dev_t sht40_dev;

static int8_t sht40_i2c_read(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, sht40_dev_t *dev) {
#if (I2C_DRV_USED == I2C_DRV_HARD)
    drv_i2c_read_series(dev->addr << 1, reg_addr, reg_len, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
#elif (I2C_DRV_USED == I2C_DRV_SOFT)
    return read_i2c_bytes(dev->addr << 1, reg_data, len);
#endif
}

static int8_t sht40_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, sht40_dev_t *dev) {
#if (I2C_DRV_USED == I2C_DRV_HARD)
    drv_i2c_write_series(dev->addr << 1, reg_addr, 2, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
#elif (I2C_DRV_USED == I2C_DRV_SOFT)
    uint8_t buff = reg_addr;
    return send_i2c_bytes(dev->addr << 1, &buff, 1);
#endif
}


static void sht40_delay(uint32_t period) {
  sleep_ms(period);
}

static uint8_t app_sht40_dev_set(uint8_t slaveID) {

    uint8_t addr = slaveID << 1;

    for (uint8_t i = 0; i < 3; i++) {
        if (scan_i2c_addr(addr) == addr) {
            sht40_dev.addr = slaveID;
            sht40_dev.delay = sht40_delay;
            sht40_dev.read = sht40_i2c_read;
            sht40_dev.write = sht40_i2c_write;
            return SENSOR_OK;
        } else {
#if UART_PRINTF_MODE && DEBUG_SENSOR
            sensor_error_codes_print_result("app_sht40_dev_set", SENSOR_ERR_ADDR_NOT_FOUND, slaveID);
#endif
        }
    }

    return SENSOR_ERR_ADDR_NOT_FOUND;
}

static uint8_t app_sht40_init() {

    sht40_dev.delay = NULL;
    sht40_dev.read = NULL;
    sht40_dev.write = NULL;
    sht40_dev.addr = 0;
    sht40_dev.id = 0;

    uint8_t ret;

    if (app_sht40_dev_set(SHT4X_I2C_ADDRESS) != SENSOR_OK) {
        if (app_sht40_dev_set(SHT4X_I2C_ADDRESS2) != SENSOR_OK) {
            if (app_sht40_dev_set(SHT4X_I2C_ADDRESS3) != SENSOR_OK) {
                return SENSOR_ERR_ADDR_NOT_FOUND;
            }
        }
    }

    ret = sht40_init(&sht40_dev);
#if UART_PRINTF_MODE && DEBUG_SENSOR
    sensor_error_codes_print_result("sht40_init", ret, sht40_dev.addr);
#endif

    return ret;
}

static void  app_sht40_set_temperature() {

    int16_t temperature = ((int32_t)(17500 * sht40_dev.raw_temp) / 65535) - 4500 + config.temperature_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
        printf("temperature: %d\r\n", temperature);
#endif

        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&temperature);
}

static void app_sht40_set_humidity() {

    int16_t humidity = ((uint32_t)(12500 * sht40_dev.raw_hum) / 65535) - 600 + config.humidity_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
    printf("humidity:    %d\r\n", humidity);
#endif
    if (humidity < 0)
        humidity = 0;
    if (humidity > 10000)
        humidity = 10000;

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&humidity);
}

static void app_sht40_measurement() {

    uint8_t ret = sht40_readSensor();

#if UART_PRINTF_MODE && DEBUG_SENSOR
    sensor_error_codes_print_result("app_sht40_measurement", ret, sht40_dev.addr);
//    printf("temperature_raw: 0x%04x\r\n", sht40_dev.raw_temp);
//    printf("humidity_raw:    0x%04x\r\n", sht40_dev.raw_hum);
#endif

    if (ret == SENSOR_OK) {
        app_sht40_set_temperature();
        app_sht40_set_humidity();
    }
}

#elif (SENSOR_USED == SENSOR_AHT20)

static aht20_dev_t aht20_dev;

static int8_t aht20_i2c_read(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, aht20_dev_t *dev) {
#if (I2C_DRV_USED == I2C_DRV_HARD)
    drv_i2c_read_series(dev->addr << 1, reg_addr, reg_len, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
#elif (I2C_DRV_USED == I2C_DRV_SOFT)
    return read_i2c_bytes(dev->addr << 1, reg_data, len);
#endif
}

static int8_t aht20_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, aht20_dev_t *dev) {
#if (I2C_DRV_USED == I2C_DRV_HARD)
    drv_i2c_write_series(dev->addr << 1, reg_addr, 2, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
#elif (I2C_DRV_USED == I2C_DRV_SOFT)
    uint8_t buff[3];
    buff[0] = reg_addr;
    if (len) {
        buff[1] = reg_data[0];
        buff[2] = reg_data[1];
        len = 3;
    } else {
        len = 1;
    }
    return send_i2c_bytes(dev->addr << 1, buff, len);
#endif
}


static void aht20_delay(uint32_t period) {
  sleep_ms(period);
}

static uint8_t app_aht20_dev_set(uint8_t slaveID) {

    uint8_t addr = slaveID << 1;
    for (uint8_t i = 0; i < 3; i++) {
        if (scan_i2c_addr(addr) == addr) {
            aht20_dev.addr = slaveID;
            aht20_dev.delay = aht20_delay;
            aht20_dev.read = aht20_i2c_read;
            aht20_dev.write = aht20_i2c_write;
            return SENSOR_OK;
        } else {
#if UART_PRINTF_MODE && DEBUG_SENSOR
            sensor_error_codes_print_result("app_aht20_dev_set", SENSOR_ERR_ADDR_NOT_FOUND, slaveID);
#endif
        }
    }

    return SENSOR_ERR_ADDR_NOT_FOUND;
}

static uint8_t app_aht20_init() {

    uint8_t ret;

    aht20_dev.delay = NULL;
    aht20_dev.read = NULL;
    aht20_dev.write = NULL;
    aht20_dev.addr = 0;
    aht20_dev.id = 0;

#if (MODULE_WATCHDOG_ENABLE)
        drv_wd_clear();
#endif

    sleep_ms(AHT20_DELAY_POWER);

    if (app_aht20_dev_set(AHT20_I2C_ADDRESS) != SENSOR_OK) return SENSOR_ERR_ADDR_NOT_FOUND;

#if (MODULE_WATCHDOG_ENABLE)
        drv_wd_clear();
#endif

    ret = aht20_init(&aht20_dev);
#if UART_PRINTF_MODE && DEBUG_SENSOR
    sensor_error_codes_print_result("aht20_init", ret, aht20_dev.addr);
#endif

    return ret;
}

static void app_aht20_set_temperature() {

    int16_t temperature = ((int32_t)(2000 * aht20_dev.raw_temp) / 1048576 - 500) * 10 + config.temperature_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
    printf("temperature: %d\r\n", temperature);
#endif

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&temperature);

    sensor.temperature = temperature;
}

static void app_aht20_set_humidity() {

    int16_t humidity = ((uint32_t)(1000 * aht20_dev.raw_hum) / 1048576) * 10 + config.humidity_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
    printf("humidity:    %d\r\n", humidity);
#endif
    if (humidity < 0)
        humidity = 0;
    if (humidity > 10000)
        humidity = 10000;

    sensor.humidity = humidity;

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&humidity);
}

static void app_aht20_measurement() {

    uint8_t ret = aht20_readSensor();

#if UART_PRINTF_MODE && DEBUG_SENSOR
    sensor_error_codes_print_result("app_aht20_measurement", ret, aht20_dev.addr);
//    printf("temperature_raw: 0x%04x\r\n", aht20_dev.raw_temp);
//    printf("humidity_raw:    0x%04x\r\n", aht20_dev.raw_hum);
#endif

    if (ret == SENSOR_OK) {
        app_aht20_set_temperature();
        app_aht20_set_humidity();
        if (sensor.temperature == -5000 && sensor.humidity == 9990) sensor.inited = false;
    }
}

#endif /* SENSOR_USED */


sensor_error_t app_sensor_init() {
    sensor_error_t ret = SENSOR_OK;

#if (SENSOR_USED == SENSOR_CHT8305)
    sensor.init = app_cht8305_init;
    sensor.measurement = app_cht8305_measurement;
    sensor.set_temperature = app_cht8305_set_temperature;
    sensor.set_humidity = app_cht8305_set_humidity;
#elif (SENSOR_USED == SENSOR_SHT30)
    sensor.init = app_sht30_init;
    sensor.measurement = app_sht30_measurement;
    sensor.set_temperature = app_sht30_set_temperature;
    sensor.set_humidity = app_sht30_set_humidity;
#elif (SENSOR_USED == SENSOR_SHT40)
    sensor.init = app_sht40_init;
    sensor.measurement = app_sht40_measurement;
    sensor.set_temperature = app_sht40_set_temperature;
    sensor.set_humidity = app_sht40_set_humidity;
#elif (SENSOR_USED == SENSOR_AHT20)
    sensor.init = app_aht20_init;
    sensor.measurement = app_aht20_measurement;
    sensor.set_temperature = app_aht20_set_temperature;
    sensor.set_humidity = app_aht20_set_humidity;
#else
#error Sensor not defined!
#endif

    sensor.inited = false;

    ret = sensor.init();

    if (ret == SENSOR_OK) sensor.inited = true;
//    printf("sensor inited: %d\r\n", sensor.inited);
    return ret;
}

void app_sensor_measurement() {
    if (!sensor.inited) {
        if (app_sensor_init() == SENSOR_OK) sensor.measurement();
    } else sensor.measurement();
}

void  app_sensor_set_temperature() {
    if (!sensor.inited) return;
    sensor.set_temperature();
}

void app_sensor_set_humidity() {
    if (!sensor.inited) return;
    sensor.set_humidity();
}

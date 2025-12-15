#include "tl_common.h"
#include "zcl_include.h"
#include "zcl_relative_humidity_measurement.h"

#if (SENSOR_USED == SENSOR_SHT30)

#include "app_sht30.h"
#include "app_i2c.h"
#include "app_config.h"
#include "app_endpoint_cfg.h"

static sht30_dev_t sht30_dev;

#if UART_PRINTF_MODE && DEBUG_SENSOR
static void sht30_error_codes_print_result(const char api_name[], int8_t rslt) {
    if (rslt != SHT30_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case SHT30_ERR_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case SHT30_ERR_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case SHT30_ERR_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case SHT30_ERR_ADDR_NOT_FOUND:
                printf("Error [%d] : Device not found at address. Address %d not correct\r\n",
                       rslt, SHT30_I2C_ADDRESS);
                break;

            case SHT30_ERR_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

            case SHT30_ERR_CRC:
                printf("Error [%d] : Invalid crc\r\n", rslt);
                break;

//            case SHT30_ERR_NOT_READY:
//                printf("Error [%d] : Device not ready. Output buffer is empty\r\n", rslt);
//                break;

            case SHT30_ERR_MODE_FAIL:
                printf("Error [%d] : The device cannot accept commands. The device is in periodic measurement mode.\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}
#endif



int8_t sht30_i2c_read(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, sht30_dev_t *dev) {
    drv_i2c_read_series(dev->addr << 1, reg_addr, reg_len, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

int8_t sht30_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, sht30_dev_t *dev) {
    drv_i2c_write_series(dev->addr << 1, reg_addr, 2, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

void sht30_delay(uint32_t period) {
  sleep_ms(period);
}



uint8_t app_sht30_init() {

    sht30_dev.delay = NULL;
    sht30_dev.read = NULL;
    sht30_dev.write = NULL;
    sht30_dev.addr = 0;
    sht30_dev.id = 0;

    uint8_t addr = (SHT30_I2C_ADDRESS << 1);
    uint8_t ret = scan_i2c_addr(addr);
    if (ret == addr) {
        sht30_dev.addr = SHT30_I2C_ADDRESS;
        sht30_dev.delay = sht30_delay;
        sht30_dev.read = sht30_i2c_read;
        sht30_dev.write = sht30_i2c_write;
        sht30_dev.repeatability = SHT30_REPEATABILITY_HIGH;
    } else {
        addr = (SHT30_I2C_ADDRESS2 << 1);
        ret = scan_i2c_addr(addr);
        if (ret == addr) {
            sht30_dev.addr = SHT30_I2C_ADDRESS2;
            sht30_dev.delay = sht30_delay;
            sht30_dev.read = sht30_i2c_read;
            sht30_dev.write = sht30_i2c_write;
            sht30_dev.repeatability = SHT30_REPEATABILITY_HIGH;
        } else {
            ret = 0;
        }
    }


    ret = sht30_init(&sht30_dev);
#if UART_PRINTF_MODE && DEBUG_SENSOR
    sht30_error_codes_print_result("sht30_init", ret);
#endif

    if (ret == SHT30_OK) {
        sht30_dev.delay(SHT30_DELAY_DEFAULT);
        ret = sht30_set_art();

        if (ret == SHT30_OK) {
            sht30_dev.delay(SHT30_DELAY_DEFAULT);
            ret = sht30_set_heater(OFF);
        }
    }

    return ret;
}

void app_sht30_measurement() {

    app_i2c_init();

    uint8_t ret = sht30_readSensor();

#if UART_PRINTF_MODE && DEBUG_SENSOR
    sht30_error_codes_print_result("app_sht30_measurement", ret);
//    printf("temperature_raw: 0x%04x\r\n", sht30_dev.raw_temp);
//    printf("humidity_raw:    0x%04x\r\n", sht30_dev.raw_hum);
#endif

    if (ret == SHT30_OK) {
        app_sht30_set_temperature();
        app_sht30_set_humidity();
    }
}

void  app_sht30_set_temperature() {

    int16_t temperature = ((int32_t)(17500 * sht30_dev.raw_temp) / 65535) - 4500 + config.temperature_offset;

#if UART_PRINTF_MODE && DEBUG_SENSOR
        printf("temperature: %d\r\n", temperature);
#endif

        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&temperature);
}

void app_sht30_set_humidity() {

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

#endif

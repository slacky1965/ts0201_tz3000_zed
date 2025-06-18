#include "app_main.h"

static cht8305_dev_t cht8305_dev;

#if UART_PRINTF_MODE && DEBUG_CHT8305
static void cht8305_error_codes_print_result(const char api_name[], int8_t rslt) {
    if (rslt != CHT8305_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case CHT8305_ERR_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case CHT8305_ERR_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case CHT8305_ERR_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case CHT8305_ERR_ADDR_NOT_FOUND:
                printf("Error [%d] : Device not found at address. Address %d not correct\r\n",
                       rslt, CHT8305_I2C_ADDRESS);
                break;

            case CHT8305_ERR_INVALID_LEN:
                printf("Error [%d] : Invalid length error. It occurs when write is done with invalid length\r\n", rslt);
                break;

//            case CHT8305_ERR_CRC:
//                printf("Error [%d] : Invalid crc\r\n", rslt);
//                break;
//
//            case CHT8305_ERR_NOT_READY:
//                printf("Error [%d] : Device not ready. Output buffer is empty\r\n", rslt);
//                break;

            case CHT8305_ERR_MODE_FAIL:
                printf("Error [%d] : The device cannot accept commands. The device is in periodic measurement mode.\r\n", rslt);
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}
#endif



int8_t cht8305_i2c_read(uint16_t reg_addr, uint32_t reg_len, uint8_t *reg_data, uint32_t len, cht8305_dev_t *dev) {
    drv_i2c_read_series(dev->addr << 1, reg_addr, reg_len, reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

int8_t cht8305_i2c_write(uint16_t reg_addr, const uint8_t *reg_data, uint32_t len, cht8305_dev_t *dev) {
    drv_i2c_write_series(dev->addr << 1, reg_addr, 1, (uint8_t*)reg_data, len);
    return (reg_i2c_status & FLD_I2C_NAK);
}

void cht8305_delay(uint32_t period) {
  sleep_ms(period);
}



uint8_t app_cht8305_init() {

    cht8305_dev.delay = NULL;
    cht8305_dev.read = NULL;
    cht8305_dev.write = NULL;
    cht8305_dev.addr = 0;
    cht8305_dev.id = 0;

    uint8_t addr = (CHT8305_I2C_ADDRESS << 1);
    uint8_t ret = scan_i2c_addr(addr);
    if (ret == addr) {
        cht8305_dev.addr = CHT8305_I2C_ADDRESS;
        cht8305_dev.delay = cht8305_delay;
        cht8305_dev.read = cht8305_i2c_read;
        cht8305_dev.write = cht8305_i2c_write;
    } else {
        addr = (CHT8305_I2C_ADDRESS2 << 1);
        ret = scan_i2c_addr(addr);
        if (ret == addr) {
            cht8305_dev.addr = CHT8305_I2C_ADDRESS2;
            cht8305_dev.delay = cht8305_delay;
            cht8305_dev.read = cht8305_i2c_read;
            cht8305_dev.write = cht8305_i2c_write;
        } else {
            addr = (CHT8305_I2C_ADDRESS3 << 1);
            ret = scan_i2c_addr(addr);
            if (ret == addr) {
                cht8305_dev.addr = CHT8305_I2C_ADDRESS3;
                cht8305_dev.delay = cht8305_delay;
                cht8305_dev.read = cht8305_i2c_read;
                cht8305_dev.write = cht8305_i2c_write;
            } else {
                addr = (CHT8305_I2C_ADDRESS4 << 1);
                ret = scan_i2c_addr(addr);
                if (ret == addr) {
                    cht8305_dev.addr = CHT8305_I2C_ADDRESS4;
                    cht8305_dev.delay = cht8305_delay;
                    cht8305_dev.read = cht8305_i2c_read;
                    cht8305_dev.write = cht8305_i2c_write;
                } else {
                    ret = 0;
                }
            }
        }
    }


    ret = cht8305_init(&cht8305_dev);
#if UART_PRINTF_MODE && DEBUG_CHT8305
    cht8305_error_codes_print_result("cht8305_init", ret);
#endif

//    printf("cht8305 ID: 0x%08x\r\n", cht8305_dev.id);

    return ret;
}

void app_cht8305_measurement() {

    int16_t temperature, humidity;

    app_i2c_init();

    uint8_t ret = cht8305_readSensor();

#if UART_PRINTF_MODE && DEBUG_CHT8305
    cht8305_error_codes_print_result("app_cht8305_measurement", ret);
#endif

    if (ret == CHT8305_OK) {
        temperature = ((int32_t)(16500 * cht8305_dev.raw_temp) / 65535) - 4000 + config.temperature_offset;
        humidity =    ((uint32_t)(10000 * cht8305_dev.raw_hum) / 65535) + config.humidity_offset;
#if UART_PRINTF_MODE && DEBUG_CHT8305
        printf("temperature: %d\r\nhumidity: %d\r\n", temperature, humidity);
#endif
        if (humidity < 0)
            humidity = 0;
        if (humidity > 10000)
            humidity = 10000;

        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&temperature);
        zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE, (uint8_t*)&humidity);

    }
}


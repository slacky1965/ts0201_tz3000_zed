#include "app_main.h"

config_t config;

static uint8_t checksum(uint8_t *data, uint16_t length) {

    uint8_t crc8 = 0;

    for(uint8_t i = 0; i < length; i++) {
        crc8 += data[i];
    }

    return crc8;
}


nv_sts_t config_save() {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

#if UART_PRINTF_MODE
    printf("Saved config\r\n");
#endif

    config.crc = checksum((uint8_t*)&config, sizeof(config_t)-1);
    st = nv_flashWriteNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(config_t), (uint8_t*)&config);

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t config_restore() {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    config_t temp_config;

    st = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(config_t), (uint8_t*)&temp_config);

    if (st == NV_SUCC && temp_config.crc == checksum((uint8_t*)&temp_config, sizeof(config_t)-1)) {

#if UART_PRINTF_MODE
        printf("Restored config\r\n");
#endif

        memcpy(&config, &temp_config, (sizeof(config_t)));
    } else {
        /* default config */
#if UART_PRINTF_MODE
        printf("Default config\r\n");
#endif
        config.read_sensors_period = DEFAULT_READ_SENSORS_PERIOD;
        config.temperature_offset = DEFAULT_TEMPERATURE_OFFSET;
        config.temperature_onoff = DEFAULT_TEMPERATURE_ONOFF;
        config.temperature_onoff_high = DEFAULT_TEMPERATURE_ONOFF_MAX;
        config.temperature_onoff_low = DEFAULT_TEMPERATURE_ONOFF_MIN;
        config.humidity_offset = DEFAULT_HUMIDITY_OFFSET;
        config.humidity_onoff = DEFAULT_HUMIDITY_ONOFF;
        config.humidity_onoff_high = DEFAULT_HUMIDITY_ONOFF_MAX;
        config.humidity_onoff_low = DEFAULT_HUMIDITY_ONOFF_MIN;
    }

    g_zcl_temperatureAttrs.read_sensors_period = config.read_sensors_period;
    g_zcl_temperatureAttrs.temperature_offset = config.temperature_offset;
    g_zcl_temperatureAttrs.temperature_onoff = config.temperature_onoff;
    g_zcl_temperatureAttrs.temperature_onoff_high = config.temperature_onoff_high;
    g_zcl_temperatureAttrs.temperature_onoff_low = config.temperature_onoff_low;
    g_zcl_humidityAttrs.humidity_offset = config.humidity_offset;
    g_zcl_humidityAttrs.humidity_onoff = config.humidity_onoff;
    g_zcl_humidityAttrs.humidity_onoff_high = config.humidity_onoff_high;
    g_zcl_humidityAttrs.humidity_onoff_low = config.humidity_onoff_low;

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}


#include "tl_common.h"
#include "sht30.h"

#if (SENSOR_USED == SENSOR_SHT30)

static sht30_dev_t *dev = NULL;

static uint8_t sht30_crc(uint8_t *data, uint16_t len)
{
    const uint8_t polynom = 0x31;
    uint8_t crc = 0xFF;
    uint16_t i, j;

    for (j = len; j != 0; --j) {
        crc ^= *data++;
        for (i = 8; i != 0; --i)
        {
            crc = (crc & 0x80) ? (crc << 1) ^ polynom : (crc<<1);
        }
    }

    return crc;
}


sht30_error_t sht30_init(sht30_dev_t *pdev) {

    if (pdev == NULL)
        return SHT30_ERR_DEV_NOT_FOUND;

    if (!pdev->addr)
        return SHT30_ERR_ADDR_NOT_FOUND;

    if (!pdev->delay)
        return SHT30_ERR_NULL_PTR;

    if (!pdev->read)
        return SHT30_ERR_NULL_PTR;

    if (!pdev->write)
        return SHT30_ERR_NULL_PTR;

    dev = pdev;

    uint8_t buff[1];

    if (dev->write(SHT30_SOFT_RESET, buff, 0, dev)) return SHT30_ERR_COMM_FAIL;

    return SHT30_OK;
}

sht30_error_t sht30_set_art() {

    if (!dev) {
        return SHT30_ERR_DEV_NOT_FOUND;
    }

    uint8_t buff[1];

    if (dev->write(SHT30_ART, buff, 0, dev)) return SHT30_ERR_COMM_FAIL;

    return SHT30_OK;
}

sht30_error_t sht30_set_heater(uint8_t enable) {

    uint16_t cmd;
    uint8_t buff[1];

    if (!dev) {
        return SHT30_ERR_DEV_NOT_FOUND;
    }

    if (enable) {
        cmd = SHT30_HEATER_ENABLE;
    } else {
        cmd = SHT30_HEATER_DISABLE;
    }

    if (dev->write(cmd, buff, 0, dev)) return SHT30_ERR_COMM_FAIL;

    return SHT30_OK;
}

sht30_error_t sht30_readSensor() {

    uint8_t buff[6] = {0};

    if (!dev) {
        return SHT30_ERR_DEV_NOT_FOUND;
    }

    if (dev->write(SHT30_READ_SENSOR, buff, 0, dev) == SHT30_OK) {

        dev->delay(SHT30_DELAY_MEASURING);
        uint8_t i = 3;

        while(i--) {
            if (dev->read(0, 0, buff, 6, dev) == SHT30_OK) {
                if (sht30_crc(buff, 2) != buff[2]) {
                    return SHT30_ERR_CRC;
                }

                if (sht30_crc(buff+3, 2) != buff[5]) {
                    return SHT30_ERR_CRC;
                }

                dev->raw_temp = (buff[0] << 8) | buff[1];
                dev->raw_hum = (buff[3] << 8) | buff[4];
                return SHT30_OK;
            }
        }
    }

    return SHT30_ERR_COMM_FAIL;
}

#endif // SENSOR_USED == SENSOR_SHT30

#include "tl_common.h"
#include "sht40.h"

#if (SENSOR_USED == SENSOR_SHT40)

static sht40_dev_t *dev = NULL;

static uint8_t sht4x_crc(uint8_t *data, uint16_t len) {
    const uint8_t POLYNOM = 0x31;
    uint8_t crc = 0xFF;
    uint16_t i;
    uint16_t j;

    for (j = len; j != 0; --j)
    {
        crc ^= *data++;
        for (i = 8; i != 0; --i)
        {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOM : (crc<<1);
        }
    }

    return crc;
}

sht40_error_t sht40_init(sht40_dev_t *pdev) {

    if (pdev == NULL)
        return SHT40_ERR_DEV_NOT_FOUND;

    if (!pdev->addr)
        return SHT40_ERR_ADDR_NOT_FOUND;

    if (!pdev->delay)
        return SHT40_ERR_NULL_PTR;

    if (!pdev->read)
        return SHT40_ERR_NULL_PTR;

    if (!pdev->write)
        return SHT40_ERR_NULL_PTR;

    dev = pdev;

    if (dev->write(SHT4X_SOFT_RESET, NULL, 0, dev)) return SHT40_ERR_COMM_FAIL;

    dev->delay(SHT4X_DELAY_SOFT_RESET);

    return SHT40_OK;
}

sht40_error_t sht40_readSensor() {

    uint8_t buff[6] = {0};

    if (!dev) {
        return SHT40_ERR_DEV_NOT_FOUND;
    }

    if (dev->write(SHT4X_HIGH_PRECISION_WITH_NO_HEATER, NULL, 0, dev) == SHT40_OK) {
        dev->delay(SHT4X_DELAY_MEASURING);
        uint8_t i = 3;

        while(i--) {
            if (dev->read(0, 0, buff, 6, dev) == SHT40_OK) {
                if (sht4x_crc(buff, 2) != buff[2]) {
                    return SHT40_ERR_CRC;
                }

                if (sht4x_crc(buff+3, 2) != buff[5]) {
                    return SHT40_ERR_CRC;
                }

                dev->raw_temp = (buff[0] << 8) | buff[1];
                dev->raw_hum = (buff[3] << 8) | buff[4];
                return SHT40_OK;
            }
        }
    }

    return SHT40_ERR_COMM_FAIL;
}


#endif /* SENSOR_USED */

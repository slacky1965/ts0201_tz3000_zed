#include "tl_common.h"
#include "aht20.h"

#if (SENSOR_USED == SENSOR_AHT20)

static aht20_dev_t *dev = NULL;

static uint8_t aht20_crc(uint8_t *data, uint8_t len) {
    uint8_t i;
    uint8_t byte;
    uint8_t crc = 0xFF;

    for (byte = 0; byte < len; byte++) {
        crc ^= data[byte];
        for (i = 8; i > 0; --i) {
            if ((crc & 0x80) != 0) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc = crc << 1;                 /* skip */
            }
        }
    }

    return crc;
}

static aht20_error_t aht20_reset_reg(uint8_t addr) {

    uint8_t buff[2] = {0};
    uint8_t regs[3] = {0};


    if (dev->write(addr, buff, 2, dev) != AHT20_OK)  return AHT20_ERR_COMM_FAIL;
    dev->delay(AHT20_DELAY_REG_RESET);
    if (dev->read(0, 0, regs, 3, dev) != AHT20_OK)  return AHT20_ERR_COMM_FAIL;
    dev->delay(AHT20_DELAY_DEFAULT);
    addr |= 0xB0;
    buff[0] = regs[1];
    buff[1] = regs[2];
    if (dev->write(addr, buff, 2, dev) != AHT20_OK)  return AHT20_ERR_COMM_FAIL;
    dev->delay(AHT20_DELAY_DEFAULT);

    return AHT20_OK;
}

aht20_error_t aht20_init(aht20_dev_t *pdev) {

    uint8_t status;

    if (pdev == NULL)
        return AHT20_ERR_DEV_NOT_FOUND;

    if (!pdev->addr)
        return AHT20_ERR_ADDR_NOT_FOUND;

    if (!pdev->delay)
        return AHT20_ERR_NULL_PTR;

    if (!pdev->read)
        return AHT20_ERR_NULL_PTR;

    if (!pdev->write)
        return AHT20_ERR_NULL_PTR;

    dev = pdev;

    if (dev->write(AHT20_SOFT_RESET, &status, 0, dev) != AHT20_OK) return AHT20_ERR_COMM_FAIL;
    dev->delay(AHT20_DELAY_SOFT_RESET);
    if (dev->read(0, 0, &status, 1, dev) != AHT20_OK) return AHT20_ERR_COMM_FAIL;

    if ((status & 0x18) != 0x18) {
        if (aht20_reset_reg(AHT20_REG_1B) != AHT20_OK) return AHT20_ERR_COMM_FAIL;
        if (aht20_reset_reg(AHT20_REG_1C) != AHT20_OK) return AHT20_ERR_COMM_FAIL;
        if (aht20_reset_reg(AHT20_REG_1E) != AHT20_OK) return AHT20_ERR_COMM_FAIL;
    }

    return AHT20_OK;
}

aht20_error_t aht20_readSensor() {

    uint8_t buff[7] = {0};
    uint8_t status;

    if (!dev) {
        return AHT20_ERR_DEV_NOT_FOUND;
    }

    buff[0] = AHT20_MEASUREMENT_DATA1;
    buff[1] = AHT20_MEASUREMENT_DATA2;

    if (dev->write(AHT20_MEASUREMENT_TRIGGER, buff, 2, dev) != AHT20_OK) return AHT20_ERR_COMM_FAIL;
    dev->delay(AHT20_DELAY_MEASURING);
    if (dev->read(0, 0, &status, 1, dev) != AHT20_OK) return AHT20_ERR_COMM_FAIL;

    if ((status & 0x80) == 0) {
        if (dev->read(0, 0, buff, 7, dev) != AHT20_OK) return AHT20_ERR_COMM_FAIL;
        if (aht20_crc(buff, 6) != buff[6]) return AHT20_ERR_CRC;

        dev->raw_hum = (((uint32_t)buff[1]) << 16) | (((uint32_t)buff[2]) << 8) | (((uint32_t)buff[3]) << 0);
        dev->raw_hum = dev->raw_hum >> 4;

        dev->raw_temp = (((uint32_t)buff[3]) << 16) | (((uint32_t)buff[4]) << 8) | (((uint32_t)buff[5]) << 0);
        dev->raw_temp = dev->raw_temp & 0xFFFFF;

        return AHT20_OK;
    }

    return AHT20_ERR_COMM_FAIL;
}



#endif /* (SENSOR_USED == SENSOR_AHT20) */

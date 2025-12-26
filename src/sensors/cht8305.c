#include "tl_common.h"
#include "cht8305.h"

#if (SENSOR_USED == SENSOR_CHT8305)

static cht8305_dev_t *dev = NULL;

cht8305_error_t cht8305_init(cht8305_dev_t *pdev) {

    if (pdev == NULL)
        return CHT8305_ERR_DEV_NOT_FOUND;

    if (!pdev->addr)
        return CHT8305_ERR_ADDR_NOT_FOUND;

    if (!pdev->delay)
        return CHT8305_ERR_NULL_PTR;

    if (!pdev->read)
        return CHT8305_ERR_NULL_PTR;

    if (!pdev->write)
        return CHT8305_ERR_NULL_PTR;

    dev = pdev;

    uint8_t buff[4];

    cht8305_error_t ret = dev->read(CHT8305_REG_MID, 1, buff, 2, dev);

    if (ret == CHT8305_OK) {
        ret = dev->read(CHT8305_REG_VID, 1, &buff[2], 2, dev);
        if (ret == CHT8305_OK) {
            dev->id = (buff[2] << 24) | (buff[3] << 16) | (buff[0] << 8) | buff[1];
            if(dev->id == ((CHT8305_VID << 16) | CHT8305_MID)) {
                buff[0] = (CHT8305_CFG_SOFT_RESET | CHT8305_CFG_MODE) >> 8;
                buff[1] = (CHT8305_CFG_SOFT_RESET | CHT8305_CFG_MODE) & 0xff;
                ret = dev->write(CHT8305_REG_CFG, buff, 2, dev);
                dev->delay(CHT8305_DELAY_CFG);
                if (ret == CHT8305_OK) {
                    buff[0] = CHT8305_CFG_MODE >> 8;
                    buff[1] = CHT8305_CFG_MODE & 0xff;
                    ret = dev->write(CHT8305_REG_CFG, buff, 2, dev);
                    dev->delay(CHT8305_DELAY_CFG);
                }
            } else {
                ret = CHT8305_ERR_ID;
            }
        }
    }

    return ret;
}

cht8305_error_t cht8305_readSensor() {

    uint8_t buff[4];
    cht8305_error_t ret = CHT8305_OK;

    if (!dev) {
        return CHT8305_ERR_DEV_NOT_FOUND;
    }

    uint8_t i = 3;
    while(i--) {
        ret = dev->write(CHT8305_REG_TMP, buff, 0, dev);
        if (ret == CHT8305_OK) break;
    }

    if (ret == CHT8305_OK) {

        dev->delay(CHT8305_DELAY_MEASURING);
        i = 3;

        while(i--) {
            if (dev->read(0, 0, buff, 4, dev) == CHT8305_OK) {
                dev->raw_temp = (buff[0] << 8) | buff[1];
                dev->raw_hum = (buff[2] << 8) | buff[3];
                return CHT8305_OK;
            }
        }
    }

    return CHT8305_ERR_COMM_FAIL;
}

#endif

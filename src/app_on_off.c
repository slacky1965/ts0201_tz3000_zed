#include "app_main.h"

#define HUMIDITY_TEST   OFF

#define IDLE        0xFF
#define TEMP_IDX   (APP_ENDPOINT1 - 1)
#define HUM_IDX    (APP_ENDPOINT2 - 1)
#define DEFAULT_REPEAT_TIME 60

static uint8_t sw_onoff[ONOFFCFG_AMT] = {IDLE, IDLE};
static uint8_t common_sw_onoff[ONOFFCFG_AMT] = {IDLE, IDLE};
static ev_timer_event_t *timerTempEvt = NULL;
static ev_timer_event_t *timerHumEvt = NULL;
static int16_t temp_save;
static uint16_t hum_save;

#if HUMIDITY_TEST
static uint16_t hum_test = 1800;        // 18%
static uint8_t hum_direction = 1;
#endif

static void cmdOnOff(uint8_t ep, uint8_t command) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;

    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    /* command 0x00 - off, 0x01 - on, 0x02 - toggle */

    switch(command) {
        case ZCL_CMD_ONOFF_OFF:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: 'off' in endPoint: %d\r\n", ep);
#endif /* UART_PRINTF_MODE */
            zcl_onOff_offCmd(ep, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_ON:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: 'on' in endPoint: %d\r\n", ep);
#endif /* UART_PRINTF_MODE */
            zcl_onOff_onCmd(ep, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_TOGGLE:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: 'toggle' in endPoint: %d\r\n", ep);
#endif /* UART_PRINTF_MODE */
            zcl_onOff_toggleCmd(ep, &dstEpInfo, FALSE);
            break;
        default:
            break;
    }
}

static int32_t temp_cmd_repeatCb(void *args) {

//    printf("temp_cmd_repeatCb()\r\n");

    if (!config.repeat_cmd) {
        timerTempEvt = NULL;
        return -1;
    }

    uint8_t ep = (uint8_t)((uint32_t)args);
    uint8_t idx = ep-1;
    bool one_device = onoff_get_one_device();

    zcl_temperatureAttr_t *tempAttrs = zcl_temperatureAttrGet();
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if (sw_onoff[idx]) {
        if (one_device && config.humidity_onoff) cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
        else if (((tempAttrs->value / 10) >= (temp_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                ((tempAttrs->value / 10) <= (temp_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
            cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
        }
    } else {
        if (one_device && config.humidity_onoff) cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
        else if (((tempAttrs->value / 10) >= (temp_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                ((tempAttrs->value / 10) <= (temp_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
            cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
        }
    }

    return 0;
}

static int32_t hum_cmd_repeatCb(void *args) {

//    printf("hum_cmd_repeatCb()\r\n");

    if (!config.repeat_cmd) {
        timerHumEvt = NULL;
        return -1;
    }

    uint8_t ep = (uint8_t)((uint32_t)args);
    uint8_t idx = ep-1;
    bool one_device = onoff_get_one_device();

#if HUMIDITY_TEST
    zcl_humidityAttr_t humAttrs_test;
    humAttrs_test.value = hum_test;
    zcl_humidityAttr_t *humAttrs = &humAttrs_test;
#else
    zcl_humidityAttr_t *humAttrs = zcl_humidityAttrGet();
#endif

    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

//    printf("ctrl hum set. hum: %d, hum_save: %d\r\n", humAttrs->value, hum_save);

    if (sw_onoff[idx]) {
        if (one_device && config.temperature_onoff) cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
        else if (((humAttrs->value / 10) >= (hum_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                ((humAttrs->value / 10) <= (hum_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
//            printf("ctrl hum set. hum: %d, hum_save: %d\r\n", humAttrs->value, hum_save);
            cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
        }
    } else {
        if (one_device && config.temperature_onoff) cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
        else if (((humAttrs->value / 10) >= (hum_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                ((humAttrs->value / 10) <= (hum_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
//            printf("ctrl hum set. hum: %d, hum_save: %d\r\n", humAttrs->value, hum_save);
            cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
        }
    }

    return 0;
}

static void proc_temp_onoff(uint8_t ep) {

    uint8_t idx = ep - 1;
    uint32_t seconds = DEFAULT_REPEAT_TIME;
    bool one_device = onoff_get_one_device();

    zcl_temperatureAttr_t *tempAttrs = zcl_temperatureAttrGet();
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if(zb_isDeviceJoinedNwk()) {

        if (!config.temperature_onoff) {
            if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
            if (!one_device) {
                if (sw_onoff[idx] == ON) {
                    cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                }
                if (sw_onoff[idx] != IDLE) {
                    sw_onoff[idx] = IDLE;
                }
            }
        } else {
            if (config.read_sensors_period <= seconds) seconds *= 1000;
            else seconds = config.read_sensors_period * 1000;
            if(!config.repeat_cmd && timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
            if (sw_onoff[idx] == IDLE) {
                temp_save = tempAttrs->value;
                if (!one_device) {
                    if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                    if (config.repeat_cmd) timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                }
//                printf("Temp value: %d, low: %d, high: %d\r\n", tempAttrs->value, tempAttrs->temperature_onoff_low, tempAttrs->temperature_onoff_high);
                if (onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) {
                    if (tempAttrs->value < tempAttrs->temperature_onoff_high) {
                        sw_onoff[idx] = ON;
                        if (!one_device) cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    } else {
                        sw_onoff[idx] = OFF;
                        if (!one_device) cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                } else {
                    if (tempAttrs->value > tempAttrs->temperature_onoff_low) {
                        sw_onoff[idx] = ON;
                        if (!one_device) cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    } else {
                        sw_onoff[idx] = OFF;
                        if (!one_device) cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                }
            } else if (sw_onoff[idx] == OFF) {
                if ((tempAttrs->value >= tempAttrs->temperature_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                        (tempAttrs->value <= tempAttrs->temperature_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
                    sw_onoff[idx] = ON;
                    temp_save = tempAttrs->value;
                    if (!one_device) {
                        if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                        if (config.repeat_cmd) timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                        cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    }
                }
            } else {
                if ((tempAttrs->value >= tempAttrs->temperature_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                        (tempAttrs->value <= tempAttrs->temperature_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
                    sw_onoff[idx] = OFF;
                    temp_save = tempAttrs->value;
                    if (!one_device) {
                        if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                        if (config.repeat_cmd) timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                        cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                }
            }
        }
    }
}

static void proc_hum_onoff(uint8_t ep) {

    uint8_t idx = ep - 1;
    uint32_t seconds = DEFAULT_REPEAT_TIME;
    bool one_device = onoff_get_one_device();

#if HUMIDITY_TEST
    zcl_humidityAttr_t humAttrs_test;
    humAttrs_test.value = hum_test;
    zcl_humidityAttr_t *humAttrs = &humAttrs_test;
    printf("humidity: %d%%\r\n", humAttrs->value / 100);
    if (hum_direction) {
        if (hum_test >= 2200) {
            hum_direction = 0;
            hum_test -= 100;
        } else {
            hum_test += 100;
        }
    } else {
        if (hum_test <= 1800) {
            hum_direction = 1;
            hum_test += 100;
        } else {
            hum_test -= 100;
        }
    }
#else
    zcl_humidityAttr_t *humAttrs = zcl_humidityAttrGet();
#endif

    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if(zb_isDeviceJoinedNwk()) {


        if (!config.humidity_onoff) {
            if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
            if (!one_device) {
                if (sw_onoff[idx] == ON) {
                    cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                }
                if (sw_onoff[idx] != IDLE) {
                    sw_onoff[idx] = IDLE;
                }
            }
        } else {
            if (config.read_sensors_period <= seconds) seconds *= 1000;
            else seconds = config.read_sensors_period * 1000;
            if(!config.repeat_cmd && timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
            if (sw_onoff[idx] == IDLE) {
                hum_save = humAttrs->value;
                if (!one_device) {
                    if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                    if (config.repeat_cmd) timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                }
//                printf("Hum value: %d, low: %d, high: %d\r\n", humAttrs->value, humAttrs->humidity_onoff_low, humAttrs->humidity_onoff_high);
                if (onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) {
                    if (humAttrs->value < humAttrs->humidity_onoff_high) {
                        sw_onoff[idx] = ON;
                        if (!one_device) cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    } else {
                        sw_onoff[idx] = OFF;
                        if (!one_device) cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                } else {
                    if (humAttrs->value > humAttrs->humidity_onoff_low) {
                        sw_onoff[idx] = ON;
                        if (!one_device) cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    } else {
                        sw_onoff[idx] = OFF;
                        if (!one_device) cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                }
            } else if (sw_onoff[idx] == OFF) {
                if ((humAttrs->value >= humAttrs->humidity_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                        (humAttrs->value <= humAttrs->humidity_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
                    sw_onoff[idx] = ON;
                    hum_save = humAttrs->value;
                    if (!one_device) {
                        if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                        if (config.repeat_cmd) timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                        cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    }
#if HUMIDITY_TEST
                    printf("ON. hum: %d%%\r\n", humAttrs->value / 100);
#endif
                }
            } else {
                if ((humAttrs->value >= humAttrs->humidity_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                        (humAttrs->value <= humAttrs->humidity_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
                    sw_onoff[idx] = OFF;
                    hum_save = humAttrs->value;
                    if (!one_device) {
                        if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                        if (config.repeat_cmd) timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                        cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
#if HUMIDITY_TEST
                    printf("OFF. hum: %d%%\r\n", humAttrs->value / 100);
#endif
                }
            }
        }
    }
}

static void procTempHumOnOff(uint8_t t_ep, uint8_t h_ep) {

    uint32_t seconds = DEFAULT_REPEAT_TIME;
    uint8_t t_idx = t_ep - 1;
    uint8_t h_idx = h_ep - 1;

    if(zb_isDeviceJoinedNwk()) {
        proc_temp_onoff(t_ep);
        proc_hum_onoff(h_ep);

        if (config.read_sensors_period <= seconds) seconds *= 1000;
        else seconds = config.read_sensors_period * 1000;

//        printf("temp: %d, hum: %d\r\n", sw_onoff[t_idx], sw_onoff[h_idx]);

        if (!config.temperature_onoff && !config.humidity_onoff) {
            if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
            if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);

            if (sw_onoff[t_idx] == ON || sw_onoff[h_idx] == ON) {
                cmdOnOff(t_ep, ZCL_CMD_ONOFF_OFF);
            }
            if (sw_onoff[t_idx] != IDLE) sw_onoff[t_idx] = IDLE;
            if (sw_onoff[h_idx] != IDLE) sw_onoff[h_idx] = IDLE;
            if (common_sw_onoff[t_idx] != IDLE) common_sw_onoff[t_idx] = IDLE;
            if (common_sw_onoff[h_idx] != IDLE) common_sw_onoff[h_idx] = IDLE;
        }

        if (!config.temperature_onoff && config.humidity_onoff) {
            if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
            if (sw_onoff[t_idx] == ON && sw_onoff[h_idx] == OFF) {
                cmdOnOff(t_ep, ZCL_CMD_ONOFF_OFF);
            }
            if (sw_onoff[t_idx] != IDLE) sw_onoff[t_idx] = IDLE;
            if (common_sw_onoff[t_idx] != IDLE) common_sw_onoff[t_idx] = IDLE;

            if (sw_onoff[h_idx] == ON && common_sw_onoff[h_idx] != ON) {
                common_sw_onoff[h_idx] = ON;
                cmdOnOff(h_ep, ZCL_CMD_ONOFF_ON);
                if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
            }

            if (sw_onoff[h_idx] == OFF && common_sw_onoff[h_idx] != OFF) {
                common_sw_onoff[h_idx] = OFF;
                cmdOnOff(h_ep, ZCL_CMD_ONOFF_OFF);
                if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
            }

            if (!timerHumEvt && config.repeat_cmd) timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)h_ep), seconds);

        }

        if (config.temperature_onoff && !config.humidity_onoff) {
            if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
            if (sw_onoff[h_idx] == ON && sw_onoff[t_idx] == OFF) {
                cmdOnOff(h_ep, ZCL_CMD_ONOFF_OFF);
            }
            if (sw_onoff[h_idx] != IDLE) sw_onoff[h_idx] = IDLE;
            if (common_sw_onoff[h_idx] != IDLE) common_sw_onoff[h_idx] = IDLE;

            if (sw_onoff[t_idx] == ON && common_sw_onoff[t_idx] != ON) {
                common_sw_onoff[t_idx] = ON;
                cmdOnOff(t_ep, ZCL_CMD_ONOFF_ON);
                if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
            }

            if (sw_onoff[t_idx] == OFF && common_sw_onoff[t_idx] != OFF) {
                common_sw_onoff[t_idx] = OFF;
                cmdOnOff(t_ep, ZCL_CMD_ONOFF_OFF);
                if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
            }

            if (!timerTempEvt && config.repeat_cmd) timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)t_ep), seconds);
        }

        if (config.temperature_onoff && config.humidity_onoff) {
            if (sw_onoff[t_idx] == ON && common_sw_onoff[t_idx] != ON) {
                common_sw_onoff[t_idx] = ON;
                cmdOnOff(t_ep, ZCL_CMD_ONOFF_ON);
                if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                if (config.repeat_cmd) timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)t_ep), seconds);
            }

            if (sw_onoff[h_idx] == ON && common_sw_onoff[h_idx] != ON) {
                common_sw_onoff[h_idx] = ON;
                cmdOnOff(h_ep, ZCL_CMD_ONOFF_ON);
                if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                if (config.repeat_cmd) timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)h_ep), seconds);
            }

            if (sw_onoff[t_idx] == OFF && common_sw_onoff[t_idx] != OFF) {
                common_sw_onoff[t_idx] = OFF;
                if (common_sw_onoff[h_idx] == OFF) {
                    cmdOnOff(t_ep, ZCL_CMD_ONOFF_OFF);
                    if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                    if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                    if (config.repeat_cmd) timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)t_ep), seconds);
                }
            }

            if (sw_onoff[h_idx] == OFF && common_sw_onoff[h_idx] != OFF) {
                common_sw_onoff[h_idx] = OFF;
                if (common_sw_onoff[t_idx] == OFF) {
                    cmdOnOff(h_ep, ZCL_CMD_ONOFF_OFF);
                    if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                    if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                    if (config.repeat_cmd) timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)h_ep), seconds);
                }
            }
        }

//        printf("t_onoff: %d, h_onoff: %d\r\n", common_sw_onoff[t_idx], common_sw_onoff[h_idx]);
    }

}

void proc_temp_hum_onoff() {

//#if UART_PRINTF_MODE && DEBUG_ONOFF
//    printf("Start OnOff control\r\n");
//#endif /* UART_PRINTF_MODE */

    if (app_edle_bind_tbl()) return;

    if (onoff_get_one_device()) {
//        printf("one device\r\n");
        procTempHumOnOff(APP_ENDPOINT1, APP_ENDPOINT2);
    } else {
//        printf("different devices\r\n");
        proc_temp_onoff(APP_ENDPOINT1);
        proc_hum_onoff(APP_ENDPOINT2);
    }
}

void reset_control_temp() {
    sw_onoff[TEMP_IDX] = IDLE;
    common_sw_onoff[TEMP_IDX] = IDLE;
}

void reset_control_hum() {
    sw_onoff[HUM_IDX] = IDLE;
    common_sw_onoff[HUM_IDX] = IDLE;
}

void repeat_timer_stop() {
    if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
    if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
    reset_control_temp();
    reset_control_hum();
}

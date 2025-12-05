#include "app_main.h"

#define IDLE        0xFF
#define TEMP_IDX   (APP_ENDPOINT1 - 1)
#define HUM_IDX    (APP_ENDPOINT2 - 1)

static uint8_t sw_onoff[ONOFFCFG_AMT] = {IDLE, IDLE};
static ev_timer_event_t *timerTempEvt = NULL;
static ev_timer_event_t *timerHumEvt = NULL;
static int16_t temp_save;
static uint16_t hum_save;

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

    uint8_t ep = (uint8_t)((uint32_t)args);
    uint8_t idx = ep-1;

    zcl_temperatureAttr_t *tempAttrs = zcl_temperatureAttrGet();
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if (sw_onoff[idx]) {
        if (((tempAttrs->value / 10) >= (temp_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                ((tempAttrs->value / 10) <= (temp_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
            cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
        }
    } else {
        if (((tempAttrs->value / 10) >= (temp_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                ((tempAttrs->value / 10) <= (temp_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
            cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
        }
    }

    return 0;
}

static int32_t hum_cmd_repeatCb(void *args) {

//    printf("hum_cmd_repeatCb()\r\n");

    uint8_t ep = (uint8_t)((uint32_t)args);
    uint8_t idx = ep-1;

    zcl_humidityAttr_t *humAttrs = zcl_humidityAttrGet();
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if (sw_onoff[idx]) {
        if (((humAttrs->value / 10) >= (hum_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                ((humAttrs->value / 10) <= (hum_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
            cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
        }
    } else {
        if (((humAttrs->value / 10) >= (hum_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                ((humAttrs->value / 10) <= (hum_save / 10) && onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
            cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
        }
    }

    return 0;
}

static void proc_temp_onoff(uint8_t ep) {

    uint8_t idx = ep - 1;
    uint32_t seconds = 60;

    zcl_temperatureAttr_t *tempAttrs = zcl_temperatureAttrGet();
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if(zb_isDeviceJoinedNwk()) {


        if (!config.temperature_onoff) {
            if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
            if (sw_onoff[idx] == ON) {
                cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
            }
            if (sw_onoff[idx] != IDLE) {
                sw_onoff[idx] = IDLE;
            }
        } else {
            if (config.read_sensors_period <= seconds) seconds *= 1000;
            else seconds = config.read_sensors_period * 1000;
            if (sw_onoff[idx] == IDLE) {
                temp_save = tempAttrs->value;
                if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
//                printf("Temp value: %d, low: %d, high: %d\r\n", tempAttrs->value, tempAttrs->temperature_onoff_low, tempAttrs->temperature_onoff_high);
                if (onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) {
                    if (tempAttrs->value < tempAttrs->temperature_onoff_high) {
                        sw_onoff[idx] = ON;
                        cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    } else {
                        sw_onoff[idx] = OFF;
                        cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                } else {
                    if (tempAttrs->value > tempAttrs->temperature_onoff_low) {
                        sw_onoff[idx] = ON;
                        cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    } else {
                        sw_onoff[idx] = OFF;
                        cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                }
            } else if (sw_onoff[idx] == OFF) {
                if ((tempAttrs->value >= tempAttrs->temperature_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                        (tempAttrs->value <= tempAttrs->temperature_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
                    sw_onoff[idx] = ON;
                    temp_save = tempAttrs->value;
                    if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                    timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                    cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                }
            } else {
                if ((tempAttrs->value >= tempAttrs->temperature_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                        (tempAttrs->value <= tempAttrs->temperature_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
                    sw_onoff[idx] = OFF;
                    temp_save = tempAttrs->value;
                    if(timerTempEvt) TL_ZB_TIMER_CANCEL(&timerTempEvt);
                    timerTempEvt = TL_ZB_TIMER_SCHEDULE(temp_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                    cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                }
            }
        }
    }
}

static void proc_hum_onoff(uint8_t ep) {

    uint8_t idx = ep - 1;
    uint32_t seconds = 60;

    zcl_humidityAttr_t *humAttrs = zcl_humidityAttrGet();
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if(zb_isDeviceJoinedNwk()) {


        if (!config.humidity_onoff) {
            if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
            if (sw_onoff[idx] == ON) {
                cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
            }
            if (sw_onoff[idx] != IDLE) {
                sw_onoff[idx] = IDLE;
            }
        } else {
            if (config.read_sensors_period <= seconds) seconds *= 1000;
            else seconds = config.read_sensors_period * 1000;
            if (sw_onoff[idx] == IDLE) {
                hum_save = humAttrs->value;
                if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
//                printf("Hum value: %d, low: %d, high: %d\r\n", humAttrs->value, humAttrs->humidity_onoff_low, humAttrs->humidity_onoff_high);
                if (onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) {
                    if (humAttrs->value < humAttrs->humidity_onoff_high) {
                        sw_onoff[idx] = ON;
                        cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    } else {
                        sw_onoff[idx] = OFF;
                        cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                } else {
                    if (humAttrs->value > humAttrs->humidity_onoff_low) {
                        sw_onoff[idx] = ON;
                        cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    } else {
                        sw_onoff[idx] = OFF;
                        cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    }
                }
            } else if (sw_onoff[idx] == OFF) {
                if ((humAttrs->value >= humAttrs->humidity_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                        (humAttrs->value <= humAttrs->humidity_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
                    sw_onoff[idx] = ON;
                    hum_save = humAttrs->value;
                    if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                    timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                    cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                }
            } else {
                if ((humAttrs->value >= humAttrs->humidity_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                        (humAttrs->value <= humAttrs->humidity_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
                    sw_onoff[idx] = OFF;
                    hum_save = humAttrs->value;
                    if(timerHumEvt) TL_ZB_TIMER_CANCEL(&timerHumEvt);
                    timerHumEvt = TL_ZB_TIMER_SCHEDULE(hum_cmd_repeatCb, (void *)((uint32_t)ep), seconds);
                    cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                }
            }
        }
    }
}

void proc_temp_hum_onoff() {

#if UART_PRINTF_MODE && DEBUG_ONOFF
    printf("Start OnOff control\r\n");
#endif /* UART_PRINTF_MODE */

    proc_temp_onoff(APP_ENDPOINT1);
    proc_hum_onoff(APP_ENDPOINT2);

}

void reset_control_temp() {
    sw_onoff[TEMP_IDX] = IDLE;
}

void reset_control_hum() {
    sw_onoff[HUM_IDX] = IDLE;
}

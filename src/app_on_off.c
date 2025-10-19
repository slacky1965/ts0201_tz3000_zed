#include "app_main.h"

static bool sw_onoff[ONOFFCFG_AMT] = {0};

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

static void proc_temp_onoff(uint8_t ep) {

    uint8_t idx = ep - 1;

    zcl_temperatureAttr_t *tempAttrs = zcl_temperatureAttrGet();
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if(zb_isDeviceJoinedNwk()) {


        if (sw_onoff[idx] && !config.temperature_onoff) {
            sw_onoff[idx] = false;
            switch(onoffCfgAttrs->switchActions) {
                case ZCL_SWITCH_ACTION_ON_OFF:
                    cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    break;
                case ZCL_SWITCH_ACTION_OFF_ON:
                    cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    break;
//                    case ZCL_SWITCH_ACTION_TOGGLE:
//                        cmdOnOff(ep, ZCL_CMD_ONOFF_TOGGLE);
//                        break;
                default:
                    break;
            }
        }

        if (config.temperature_onoff) {
            if (!sw_onoff[idx]) {
                if ((tempAttrs->value >= tempAttrs->temperature_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                        (tempAttrs->value <= tempAttrs->temperature_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
                    sw_onoff[idx] = true;
                    cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                }
            } else {
                if ((tempAttrs->value >= tempAttrs->temperature_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                        (tempAttrs->value <= tempAttrs->temperature_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
                    sw_onoff[idx] = false;
                    cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                }
            }
        }
    }
}

static void proc_hum_onoff(uint8_t ep) {

    uint8_t idx = ep - 1;

    zcl_humidityAttr_t *humAttrs = zcl_humidityAttrGet();
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
    onoffCfgAttrs += idx;

    if(zb_isDeviceJoinedNwk()) {


        if (sw_onoff[idx] && !config.humidity_onoff) {
            sw_onoff[idx] = false;
            switch(onoffCfgAttrs->switchActions) {
                case ZCL_SWITCH_ACTION_ON_OFF:
                    cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                    break;
                case ZCL_SWITCH_ACTION_OFF_ON:
                    cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                    break;
//                    case ZCL_SWITCH_ACTION_TOGGLE:
//                        cmdOnOff(ep, ZCL_CMD_ONOFF_TOGGLE);
//                        break;
                default:
                    break;
            }
        }

        if (config.humidity_onoff) {
//            printf("sw_onoff: %d\r\n", sw_onoff[idx]);
            if (!sw_onoff[idx]) {
                if ((humAttrs->value >= humAttrs->humidity_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON) ||
                        (humAttrs->value <= humAttrs->humidity_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF)) {
                    sw_onoff[idx] = true;
                    cmdOnOff(ep, ZCL_CMD_ONOFF_ON);
                }
            } else {
                if ((humAttrs->value >= humAttrs->humidity_onoff_high &&
                        onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_ON_OFF) ||
                        (humAttrs->value <= humAttrs->humidity_onoff_low &&
                                onoffCfgAttrs->switchActions == ZCL_SWITCH_ACTION_OFF_ON)) {
                    sw_onoff[idx] = false;
                    cmdOnOff(ep, ZCL_CMD_ONOFF_OFF);
                }
            }
        }
    }
}

void proc_temp_hum_onoff() {

    proc_temp_onoff(APP_ENDPOINT1);
    proc_hum_onoff(APP_ENDPOINT2);

}


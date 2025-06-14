#include "app_main.h"

static bool sw_onoff = false;

static void cmdOnOff(uint8_t command) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;

    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    /* command 0x00 - off, 0x01 - on, 0x02 - toggle */

    switch(command) {
        case ZCL_CMD_ONOFF_OFF:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: off\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_offCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_ON:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: on\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_onCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_TOGGLE:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: toggle\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_toggleCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        default:
            break;
    }
}

void proc_temp_hum_onoff() {

    uint16_t attr_len, rh;
    int16_t temp;

    bool temperature_ctrl_off = false;
    bool temperature_ctrl_on = false;
    bool humidity_ctrl_off = false;
    bool humidity_ctrl_on = false;

    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();

    if(zb_isDeviceJoinedNwk()) {

        zcl_getAttrVal(APP_ENDPOINT1,
                       ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
                       ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE,
                       &attr_len,
                       (uint8_t*)&temp);

        zcl_getAttrVal(APP_ENDPOINT1,
                       ZCL_CLUSTER_MS_RELATIVE_HUMIDITY,
                       ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE,
                       &attr_len,
                       (uint8_t*)&rh);

        if (config.temperature_onoff) {
            if (temp >= config.temperature_onoff_high)
                temperature_ctrl_on = true;
            if (temp <= config.temperature_onoff_low)
                temperature_ctrl_off = true;
            if (temperature_ctrl_off && temperature_ctrl_on)
                temperature_ctrl_on = false;
        }

        if (config.humidity_onoff) {
            if (rh >= config.humidity_onoff_high)
                humidity_ctrl_on = true;
            if (rh <= config.humidity_onoff_low)
                humidity_ctrl_off = true;
            if (humidity_ctrl_off && humidity_ctrl_on)
                humidity_ctrl_on = false;
        }

        if (sw_onoff && !config.temperature_onoff && !config.humidity_onoff) {
            sw_onoff = false;
            switch(onoffCfgAttrs->switchActions) {
                case ZCL_SWITCH_ACTION_ON_OFF:
                    cmdOnOff(ZCL_CMD_ONOFF_ON);
                    break;
                case ZCL_SWITCH_ACTION_OFF_ON:
                    cmdOnOff(ZCL_CMD_ONOFF_OFF);
                    break;
//                    case ZCL_SWITCH_ACTION_TOGGLE:
//                        cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                        break;
                default:
                    break;
            }
        }


        if ((temperature_ctrl_on && config.temperature_onoff) || (humidity_ctrl_on && config.humidity_onoff)) {
            if (!sw_onoff) {
                sw_onoff = true;
#if UART_PRINTF_MODE && DEBUG_ONOFF
                printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
#endif /* UART_PRINTF_MODE */
                switch(onoffCfgAttrs->switchActions) {
                    case ZCL_SWITCH_ACTION_ON_OFF:
                        cmdOnOff(ZCL_CMD_ONOFF_OFF);
                        break;
                    case ZCL_SWITCH_ACTION_OFF_ON:
                        cmdOnOff(ZCL_CMD_ONOFF_ON);
                        break;
//                    case ZCL_SWITCH_ACTION_TOGGLE:
//                        cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                        break;
                    default:
                        break;
                }
            }
        }

        if ((temperature_ctrl_off && config.temperature_onoff && !humidity_ctrl_on) || (humidity_ctrl_off && config.humidity_onoff && !temperature_ctrl_on)) {
            if (sw_onoff) {
                sw_onoff = false;
#if UART_PRINTF_MODE && DEBUG_ONOFF
                printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
#endif /* UART_PRINTF_MODE */
                switch(onoffCfgAttrs->switchActions) {
                    case ZCL_SWITCH_ACTION_ON_OFF:
                        cmdOnOff(ZCL_CMD_ONOFF_ON);
                        break;
                    case ZCL_SWITCH_ACTION_OFF_ON:
                        cmdOnOff(ZCL_CMD_ONOFF_OFF);
                        break;
//                    case ZCL_SWITCH_ACTION_TOGGLE:
//                        cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                        break;
                    default:
                        break;
                }
            }
        }
    }

//    printf("sw_onoff: %d, temperature_ctrl_off: %d, temperature_ctrl_on: %d, humidity_ctrl_off: %d, humidity_ctrl_on: %d\r\n", sw_onoff, temperature_ctrl_off, temperature_ctrl_on, humidity_ctrl_off, humidity_ctrl_on);
}


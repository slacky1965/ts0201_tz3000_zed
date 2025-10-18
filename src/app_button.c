#include "app_main.h"

static int32_t net_steer_start_offCb(void *args) {

    g_appCtx.net_steer_start = false;

    light_blink_stop();

    return -1;
}

static void buttonKeepPressed(u8 btNum) {
    g_appCtx.button[btNum-1].state = APP_FACTORY_NEW_DOING;
    g_appCtx.button[btNum-1].ctn = 0;

    if(btNum == VK_SW1) {
#if UART_PRINTF_MODE && DEBUG_BUTTON
        printf("The button was keep pressed for 5 seconds\r\n");
#endif


        zb_factoryReset();

        g_appCtx.net_steer_start = true;
        TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
        light_blink_start(90, 100, 1000);

    }
}


static void buttonSinglePressed(u8 btNum) {

    switch (btNum) {
        case VK_SW1:
#if UART_PRINTF_MODE && DEBUG_BUTTON
            printf("Button push 1 time\r\n");
#endif
            if(zb_isDeviceJoinedNwk()) {
                app_setPollRate(TIMEOUT_20SEC);
                TL_ZB_TIMER_SCHEDULE(forcedReportCb, NULL, TIMEOUT_100MS);
            } else {
                zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
            }
            break;
        default:
            break;
    }
}

//static void buttonDoublePressed(u8 btNum) {
//    printf("Command double click\r\n");
//}
//
//static void buttonTriplePressed(u8 btNum) {
//    printf("Command triple click\r\n");
//}
//
//static void buttonQuadruplePressed(u8 btNum) {
//    printf("Command quadruple click\r\n");
//}


static void buttonCheckCommand(uint8_t btNum) {
    g_appCtx.button[btNum-1].state = APP_STATE_NORMAL;

    if (g_appCtx.button[btNum-1].ctn == 1) {
        buttonSinglePressed(btNum);
//    } else if (g_appCtx.button[btNum-1].ctn == 2) {
//        buttonDoublePressed(btNum);
//    } else if (g_appCtx.button[btNum-1].ctn == 3) {
//        buttonTriplePressed(btNum);
//    } else if (g_appCtx.button[btNum-1].ctn == 4) {
//        buttonQuadruplePressed(btNum);
    }

    g_appCtx.button[btNum-1].ctn = 0;
}


void keyScan_keyPressedCB(kb_data_t *kbEvt) {

    u8 keyCode = kbEvt->keycode[0];

    if(keyCode != 0xff) {
        g_appCtx.button[keyCode-1].pressed_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_FACTORY_NEW_SET_CHECK;
        g_appCtx.button[keyCode-1].ctn++;
        light_blink_start(1, 30, 1);
        if (zb_isDeviceJoinedNwk()) {
        }
    }
}


void keyScan_keyReleasedCB(u8 keyCode){
    if (keyCode != 0xff) {
        g_appCtx.button[keyCode-1].released_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_STATE_RELEASE;

//        g_appCtx.button[keyCode-1].state = APP_STATE_NORMAL;
    }
}

void button_handler(void) {
    static u8 valid_keyCode = 0xff;

    for (uint8_t i = 0; i < MAX_BUTTON_NUM; i++) {
        if (g_appCtx.button[i].state == APP_FACTORY_NEW_SET_CHECK) {
            if(clock_time_exceed(g_appCtx.button[i].pressed_time, TIMEOUT_TICK_5SEC)) {
                buttonKeepPressed(i+1);
            }
        }

        if (g_appCtx.button[i].state == APP_STATE_RELEASE) {
            if(clock_time_exceed(g_appCtx.button[i].released_time, TIMEOUT_TICK_250MS)) {
                buttonCheckCommand(i+1);
            }

        }
    }

    if(kb_scan_key(0, 1)){
        if(kb_event.cnt){
            g_appCtx.keyPressed = 1;
            keyScan_keyPressedCB(&kb_event);
            if(kb_event.cnt == 1){
                valid_keyCode = kb_event.keycode[0];
            }
        }else{
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            g_appCtx.keyPressed = 0;
        }
    }
}

u8 button_idle() {

    if (g_appCtx.keyPressed) {
        return true;
    }

    for (uint8_t i = 0; i < MAX_BUTTON_NUM; i++) {
        if (g_appCtx.button[i].ctn) return true;
    }

    return false;
}

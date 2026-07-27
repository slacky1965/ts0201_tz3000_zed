#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

uint32_t mcuBootAddrGet(void);

void start_message() {
#ifdef ZCL_OTA
        APP_DEBUG(UART_PRINTF_MODE, "OTA mode enabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#else
        APP_DEBUG(UART_PRINTF_MODE, "OTA mode desabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif

#if UART_PRINTF_MODE
    const uint8_t version[] = ZCL_BASIC_SW_BUILD_ID;
    APP_DEBUG(UART_PRINTF_MODE, "Firmware version: %s\r\n", version+1);
#if (BOARD == BOARD_HXDZ_ZBWSD_V02)
    APP_DEBUG(UART_PRINTF_MODE, "Device is \"TS0201 _TZ3000_xr3htd96\"\r\n");
#elif (BOARD == BOARD_IHSENO_IC_V0)
    APP_DEBUG(UART_PRINTF_MODE, "Device is \"TS0201 _TZ3000_dowj6gyi\"\r\n");
#elif (BOARD == BOARD_ZBEACON)
    APP_DEBUG(UART_PRINTF_MODE, "Device is \"TS0201 Zbeacon\"\r\n");
#elif (BOARD == BOARD_ZG_227ZS)
    APP_DEBUG(UART_PRINTF_MODE, "Device is \"ZG-227Z HOBEIAN\"\r\n");
#elif (BOARD == BOARD_TH01_ZBEACON)
    APP_DEBUG(UART_PRINTF_MODE, "Device is \"TH01 Zbeacon\"\r\n");
#elif (BOARD == BOARD_Z_WXD)
    APP_DEBUG(UART_PRINTF_MODE, "Device is \"Z WXD\"\r\n");
#else
#error BOARD not defined!
#endif

#if (SENSOR_USED == SENSOR_CHT8305)
    APP_DEBUG(UART_PRINTF_MODE, "The sensor used is CHT8305\r\n");
#elif (SENSOR_USED == SENSOR_SHT30)
    APP_DEBUG(UART_PRINTF_MODE, "The sensor used is SHT30\r\n");
#elif (SENSOR_USED == SENSOR_SHT40)
    APP_DEBUG(UART_PRINTF_MODE, "The sensor used is SHT40\r\n");
#elif (SENSOR_USED == SENSOR_AHT20)
    APP_DEBUG(UART_PRINTF_MODE, "The sensor used is AHT20\r\n");
#else
#error SENSOR_USED not defined!
#endif


#endif
}

int32_t delayedMcuResetCb(void *arg) {

    //printf("mcu reset\r\n");
    zb_resetDevice();
    return -1;
}

int32_t delayedFactoryResetCb(void *arg) {

    zb_resetDevice2FN();
    zb_deviceFactoryNewSet(true);

    g_appCtx.timerFactoryReset = NULL;

//    printf("Cb Factory new: %s\r\n", zb_isDeviceFactoryNew()?"yes":"no");


    return -1;
}

int32_t delayedFullResetCb(void *arg) {

    //printf("full reset\r\n");
    return -1;
}

int32_t set_pollRateCb(void *args) {

    zb_setPollRate(POLL_RATE * g_appCtx.read_sensor_period);

//    printf("set_pollRateCb: %d\r\n", g_appCtx.read_sensor_period);

    g_appCtx.timerSetPollRateEvt = NULL;
    return -1;
}

void app_setPollRate(uint32_t sec) {

//    printf("app_setPollRate. sec: %d\r\n", sec);
    zb_setPollRate(POLL_RATE * 3);
    if (g_appCtx.timerSetPollRateEvt) {
        TL_ZB_TIMER_CANCEL(&g_appCtx.timerSetPollRateEvt);
    }
    g_appCtx.timerSetPollRateEvt = TL_ZB_TIMER_SCHEDULE(set_pollRateCb, NULL, sec /*TIMEOUT_20SEC*/);

}


//int32_t no_joinedCb(void *arg) {
//
//    if (!zb_isDeviceJoinedNwk()) {
//
//        if (tl_stackBusy() || !zb_isTaskDone()) {
//
////            printf("tl_stackBusy: %s,  zb_isTaskDone: %d\r\n", tl_stackBusy()?"true":"false", zb_isTaskDone());
//            return TIMEOUT_1MIN;
//        }
//
//#if UART_PRINTF_MODE && DEBUG_PM
//        printf("Without network more then 30 minutes! Deep sleep ...\r\n");
//#endif
//
//        drv_pm_wakeupPinLevelChange(pin_PmCfg, sizeof(pin_PmCfg)/sizeof(drv_pm_pinCfg_t));
//
//        apsCleanToStopSecondClock();
//
//        drv_disable_irq();
//        rf_paShutDown();
//        drv_pm_deepSleep_frameCnt_set(ss_outgoingFrameCntGet());
//
//        drv_pm_longSleep(PM_SLEEP_MODE_DEEPSLEEP, PM_WAKEUP_SRC_PAD, 1);
//    }
//
//    g_appCtx.timerNoJoinedEvt = NULL;
//    return -1;
//}

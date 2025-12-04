#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

uint32_t mcuBootAddrGet(void);

void start_message() {
#ifdef ZCL_OTA
#if UART_PRINTF_MODE
        printf("OTA mode enabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif /* UART_PRINTF_MODE */
#else
#if UART_PRINTF_MODE
    printf("OTA mode desabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif /* UART_PRINTF_MODE */
#endif

#if UART_PRINTF_MODE
    const uint8_t version[] = ZCL_BASIC_SW_BUILD_ID;
    printf("Firmware version: %s\r\n", version+1);
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

//    printf("set_pollRateCb\r\n");

    zb_setPollRate(POLL_RATE * 5); //config.read_sensors_period);

    g_appCtx.timerSetPollRateEvt = NULL;
    return -1;
}

void app_setPollRate(uint32_t sec) {

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

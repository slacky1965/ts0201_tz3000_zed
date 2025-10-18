#include "app_main.h"

analog_reg_time_t analog_reg_time = {0};

uint32_t last_timer_duration = 0;

void app_pm_lowPowerEnter(void) {
#if PM_ENABLE
    drv_pm_wakeup_src_e wakeupSrc = PM_WAKEUP_SRC_PAD;
    u32 sleepTime = 0;
    bool longSleep = 0;

#if !defined(__PROJECT_TL_BOOT_LOADER__) && !defined(__PROJECT_TL_SNIFFER__)
    if(tl_stackBusy() || !zb_isTaskDone()){
        return;
    }

    apsCleanToStopSecondClock();
#endif

    u32 r = drv_disable_irq();

    ev_timer_event_t *timerEvt = ev_timer_nearestGet();
    if(timerEvt){
        wakeupSrc |= PM_WAKEUP_SRC_TIMER;
        sleepTime = timerEvt->timeout;

        if(sleepTime){
            last_timer_duration = sleepTime;
            if(sleepTime > PM_NORMAL_SLEEP_MAX){
                longSleep = 1;
            }else{
                longSleep = 0;
            }
        }else{
            drv_restore_irq(r);
            return;
        }
    }

    printf("sleepTime: %d, longSleep: %d, period: %d\r\n", sleepTime, longSleep, timerEvt?timerEvt->period:-1);

#if defined(MCU_CORE_826x)
    drv_pm_sleep_mode_e sleepMode = (wakeupSrc & PM_WAKEUP_SRC_TIMER) ? PM_SLEEP_MODE_SUSPEND : PM_SLEEP_MODE_DEEPSLEEP;
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    drv_pm_sleep_mode_e sleepMode = (wakeupSrc & PM_WAKEUP_SRC_TIMER) ? PM_SLEEP_MODE_DEEP_WITH_RETENTION : PM_SLEEP_MODE_DEEPSLEEP;
#endif

#if !defined(__PROJECT_TL_BOOT_LOADER__) && !defined(__PROJECT_TL_SNIFFER__)
    rf_paShutDown();
    if(sleepMode == PM_SLEEP_MODE_DEEPSLEEP){
        drv_pm_deepSleep_frameCnt_set(ss_outgoingFrameCntGet());
    }
#endif

    if(!longSleep){
        drv_pm_sleep(sleepMode, wakeupSrc, sleepTime);
    }else{
        drv_pm_longSleep(sleepMode, wakeupSrc, sleepTime);
    }

    drv_restore_irq(r);
#endif
}


//void app_pm_lowPowerEnter() {
//#if PM_ENABLE
//    drv_pm_wakeup_src_e wakeupSrc = PM_WAKEUP_SRC_PAD;
//    u32 sleepTime = 0;
//    bool longSleep = 0;
//
//#if !defined(__PROJECT_TL_BOOT_LOADER__) && !defined(__PROJECT_TL_SNIFFER__)
//    if(tl_stackBusy() || !zb_isTaskDone()){
//        return;
//    }
//
//    apsCleanToStopSecondClock();
//#endif
//
//    u32 r = drv_disable_irq();
//
//    ev_timer_event_t *timerEvt = ev_timer_nearestGet();
//    if(timerEvt){
//        wakeupSrc |= PM_WAKEUP_SRC_TIMER;
//        sleepTime = timerEvt->timeout;
//
//        if(sleepTime){
//            if(sleepTime >= ((config.read_sensors_period - 1) * 1000) && !g_appCtx.not_sleep){
//                longSleep = 1;
//            }else{
//                longSleep = 0;
//            }
//        }else{
//            drv_restore_irq(r);
//            return;
//        }
//    }
//
//    printf("sleepTime: %d, longSleep: %d\r\n", sleepTime, longSleep);
//
//#if defined(MCU_CORE_826x)
//    drv_pm_sleep_mode_e sleepMode = (wakeupSrc & PM_WAKEUP_SRC_TIMER) ? PM_SLEEP_MODE_SUSPEND : PM_SLEEP_MODE_DEEPSLEEP;
//#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
//    drv_pm_sleep_mode_e sleepMode = ((wakeupSrc & PM_WAKEUP_SRC_TIMER) && !longSleep) ? PM_SLEEP_MODE_DEEP_WITH_RETENTION : PM_SLEEP_MODE_DEEPSLEEP;
//#endif
//
//#if !defined(__PROJECT_TL_BOOT_LOADER__) && !defined(__PROJECT_TL_SNIFFER__)
//    rf_paShutDown();
//    if(sleepMode == PM_SLEEP_MODE_DEEPSLEEP){
//        drv_pm_deepSleep_frameCnt_set(ss_outgoingFrameCntGet());
//    }
//#endif
//
//    if(!longSleep){
//        printf("not long sleep\r\n");
//        drv_pm_sleep(sleepMode, wakeupSrc, sleepTime);
//    }else{
//        printf("long sleep\r\n");
//        drv_pm_longSleep(sleepMode, wakeupSrc, sleepTime);
//    }
//
//    drv_restore_irq(r);
//#endif
//}
//

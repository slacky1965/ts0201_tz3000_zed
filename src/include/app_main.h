#ifndef SRC_INCLUDE_APP_MAIN_H_
#define SRC_INCLUDE_APP_MAIN_H_

#include "tl_common.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"

#include "zcl_relative_humidity_measurement.h"
#include "zcl_onoffSwitchCfg.h"
#include "zcl_custom_attr.h"
#include "app_config.h"
#include "app_button.h"
#include "app_led.h"
#include "app_pm.h"
#include "app_on_off.h"
#include "app_battery.h"
#include "app_reporting.h"
#include "app_endpoint_cfg.h"
#include "app_utility.h"
#include "app_i2c.h"
#include "app_cht8305.h"

typedef struct {
    uint8_t keyType; /* CERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
                        SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
    uint8_t key[16]; /* the key used */
} app_linkKey_info_t;

typedef struct {
    ev_timer_event_t *bdbFBTimerEvt;
    ev_timer_event_t *timerForcedReportEvt;
    ev_timer_event_t *timerFactoryReset;
    ev_timer_event_t *timerSetPollRateEvt;
    ev_timer_event_t *timerBatteryEvt;
    ev_timer_event_t *timerLedEvt;
    ev_timer_event_t *timerNoJoinedEvt;

//    uint32_t short_poll;
//    uint32_t long_poll;
//    uint32_t current_poll;

//    button_t button;
    button_t button[MAX_BUTTON_NUM];
//    u32 keyPressedTime;
    u8  keyPressed;

    uint16_t ledOnTime;
    uint16_t ledOffTime;
    uint8_t  oriSta;     //original state before blink
    uint8_t  sta;        //current state in blink
    uint8_t  times;      //blink times

    uint32_t read_sensor_time;

    uint32_t time_without_joined;

    bool net_steer_start;

    app_linkKey_info_t tcLinkKey;
} app_ctx_t;


extern app_ctx_t g_appCtx;

extern bdb_appCb_t g_zbBdbCb;

extern bdb_commissionSetting_t g_bdbCommissionSetting;

void app_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t app_basicCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_groupCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_temperatureCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);
status_t app_humidityCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload);


void app_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void app_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void app_otaProcessMsgHandler(uint8_t evt, uint8_t status);
void app_wakeupPinLevelChange();


#endif /* SRC_INCLUDE_APP_MAIN_H_ */

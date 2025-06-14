#include "tl_common.h"
#include "zcl_include.h"
#include "ota.h"

#include "app_main.h"

app_ctx_t g_appCtx = {
        .bdbFBTimerEvt = NULL,
        .timerForcedReportEvt = NULL,
        .timerNoJoinedEvt = NULL,
        .timerSetPollRateEvt = NULL,
        .oriSta = false,
        .time_without_joined = 0,
        .net_steer_start = false,
        .read_sensor_time = 0,
};

//uint32_t count_restart = 0;

#ifdef ZCL_OTA
extern ota_callBack_t app_otaCb;

//running code firmware information
ota_preamble_t app_otaInfo = {
    .fileVer            = FILE_VERSION,
    .imageType          = IMAGE_TYPE,
    .manufacturerCode   = MANUFACTURER_CODE_TELINK,
};
#endif


//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
    bdb_zdoStartDevCnf,//start device cnf cb
    NULL,//reset cnf cb
    NULL,//device announce indication cb
    app_leaveIndHandler,//leave ind cb
    app_leaveCnfHandler,//leave cnf cb
    NULL,//nwk update ind cb
    NULL,//permit join ind cb
    NULL,//nlme sync cnf cb
    NULL,//tc join ind cb
    NULL,//tc detects that the frame counter is near limit
};


/**
 *  @brief Definition for BDB finding and binding cluster
 */
uint16_t bdb_findBindClusterList[] =
{
    ZCL_CLUSTER_GEN_ON_OFF,
};

/**
 *  @brief Definition for BDB finding and binding cluster number
 */
#define FIND_AND_BIND_CLUSTER_NUM       (sizeof(bdb_findBindClusterList)/sizeof(bdb_findBindClusterList[0]))

/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
    .linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
    .linkKey.tcLinkKey.key = (uint8_t *)tcLinkKeyCentralDefault,             //can use unique link key stored in NV

    .linkKey.distributeLinkKey.keyType = MASTER_KEY,
    .linkKey.distributeLinkKey.key = (uint8_t *)linkKeyDistributedMaster,    //use linkKeyDistributedCertification before testing

    .linkKey.touchLinkKey.keyType = MASTER_KEY,
    .linkKey.touchLinkKey.key = (uint8_t *)touchLinkKeyMaster,               //use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
    .touchlinkEnable = 1,                                               /* enable touch-link */
#else
    .touchlinkEnable = 0,                                               /* disable touch-link */
#endif
    .touchlinkChannel = DEFAULT_CHANNEL,                                /* touch-link default operation channel for target */
    .touchlinkLqiThreshold = 0xA0,                                      /* threshold for touch-link scan req/resp command */
};

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */
drv_pm_pinCfg_t g_switchPmCfg[] = {
    {
        BUTTON1,
        PM_WAKEUP_LEVEL_LOW
    },
};
#endif

/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
    /* Initialize ZB stack */
    zb_init();

    /* Register stack CB */
    zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{

#if ZCL_POLL_CTRL_SUPPORT
    af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_PERIODICALLY);
#else
    af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED);
#endif

    /* Initialize ZCL layer */
    /* Register Incoming ZCL Foundation command/response messages */
    zcl_init(app_zclProcessIncomingMsg);

    /* register endPoint */
    af_endpointRegister(APP_ENDPOINT1, (af_simple_descriptor_t *)&app_ep1Desc, zcl_rx_handler, NULL);

    zcl_reportingTabInit();
    zcl_onOffCfgAttr_restore();
    config_restore();

    /* Register ZCL specific cluster information */
    zcl_register(APP_ENDPOINT1, APP_EP1_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_appEp1ClusterList);

#if ZCL_OTA_SUPPORT
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&app_ep1Desc, &app_otaInfo, &app_otaCb);
#endif

    batteryCb(NULL);

#if DEBUG_BATTERY
    g_appCtx.timerBatteryEvt = TL_ZB_TIMER_SCHEDULE(batteryCb, NULL, 5000);
#else
    g_appCtx.timerBatteryEvt = TL_ZB_TIMER_SCHEDULE(batteryCb, NULL, BATTERY_TIMER_INTERVAL);
#endif

    app_i2c_init();

    app_cht8305_init();

//#if UART_PRINTF_MODE
//    printf("IMAGE_TYPE: 0x%x\r\n", IMAGE_TYPE);
//    printf("FILE_VERSION: 0x%x\r\n", FILE_VERSION);
//#endif

}

void app_task(void) {

    button_handler();

    if (zb_isDeviceJoinedNwk()) {
        if (clock_time_exceed(g_appCtx.read_sensor_time, config.read_sensors_period * 1000 * 1000)) {
            g_appCtx.read_sensor_time = clock_time();
            app_cht8305_measurement();
            light_blink_stop();
            light_blink_start(1, 30, 30);

            proc_temp_hum_onoff();
        }
    }

    if(bdb_isIdle()) {
        report_handler();
#if PM_ENABLE
        button_handler();
        if(!button_idle()) {
            drv_pm_lowPowerEnter();
        }
#endif
    }
}

extern volatile uint16_t T_evtExcept[4];

static void appSysException(void) {

#if UART_PRINTF_MODE
    printf("app_sysException, line: %d, event: 0x%02x, reset\r\n", T_evtExcept[0], T_evtExcept[1]);
#endif

#if 1
    SYSTEM_RESET();
#else
    led_on(LED_STATUS);
    while(1);
#endif
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
#if UART_PRINTF_MODE
//    printf("[%d] isRetention: %s\r\n", count_restart++, isRetention?"true":"false");
#endif /* UART_PRINTF_MODE */

    /* Initialize LEDs*/
    light_init();

#if PA_ENABLE
    rf_paInit(PA_TX, PA_RX);
#endif

#if ZBHCI_EN
    zbhciInit();
#endif

#if PM_ENABLE
    drv_pm_wakeupPinConfig(g_switchPmCfg, sizeof(g_switchPmCfg)/sizeof(drv_pm_pinCfg_t));
#endif

    if(!isRetention) {

        start_message();

#ifdef CHECK_BOOTLOADER
        bootloader_check();
#endif


        /* Initialize Stack */
        stack_init();

        /* Initialize user application */
        user_app_init();

        /* Register except handler for test */
        sys_exceptHandlerRegister(appSysException);

        /* User's Task */
#if ZBHCI_EN
        ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
        ev_on_poll(EV_POLL_IDLE, app_task);

        /* Load the pre-install code from flash */
        if(bdb_preInstallCodeLoad(&g_appCtx.tcLinkKey.keyType, g_appCtx.tcLinkKey.key) == RET_OK){
            g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_appCtx.tcLinkKey.keyType;
            g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_appCtx.tcLinkKey.key;
        }

        bdb_findBindMatchClusterSet(FIND_AND_BIND_CLUSTER_NUM, bdb_findBindClusterList);

        /* Set default reporting configuration */
        uint8_t reportableChange = 0x00;
        bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_GEN_POWER_CFG,
                ZCL_ATTRID_BATTERY_VOLTAGE, REPORTING_MAX, 0, (uint8_t *)&reportableChange);
        bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_GEN_POWER_CFG,
                ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, REPORTING_MAX, 0, (uint8_t *)&reportableChange);

        uint16_t reportableChange_u16 = 10;
        bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
                ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE, 10, REPORTING_MAX, (uint8_t *)&reportableChange_u16);
        bdb_defaultReportingCfg(APP_ENDPOINT1, HA_PROFILE_ID, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY,
                ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE, 10, REPORTING_MAX, (uint8_t *)&reportableChange_u16);

        /* Initialize BDB */
        uint8_t repower = drv_pm_deepSleep_flag_get() ? 0 : 1;
        bdb_init((af_simple_descriptor_t *)&app_ep1Desc, &g_bdbCommissionSetting, &g_zbBdbCb, repower);

    }else{
        /* Re-config phy when system recovery from deep sleep with retention */
        mac_phyReconfig();

    }
}


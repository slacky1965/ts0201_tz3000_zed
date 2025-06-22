#include "app_main.h"

static void forceReportBatVoltage(void *args) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE);
    if (pAttrEntry)
        zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                ZCL_CLUSTER_GEN_POWER_CFG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

}

static void forceReportBatPercent(void *args) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);
    if (pAttrEntry)
        zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                ZCL_CLUSTER_GEN_POWER_CFG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

}

static void forceReportTemperature(void *args) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE);
    if (pAttrEntry)
        zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

}

static void forceReportHumidity(void *args) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE);
    if (pAttrEntry)
        zcl_sendReportCmd(APP_ENDPOINT1, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
}

int32_t forcedReportCb(void *arg) {

//    for (uint8_t i = 0; i < APS_BINDING_TABLE_NUM; i++) {
//        if (g_apsBindingTbl[i].used) {
//            u8 r = irq_disable();
//            printf("bind_tbl\r\n");
//            printf("addr_mode: 0x%02x, clId: 0x%04x, ",
//                    g_apsBindingTbl[i].dstAddrMode,
//                    g_apsBindingTbl[i].clusterId);
//
//            printf("addr: ");
//            for (uint8_t ii = 0; ii < 8; ii++) {
//                printf("0x%02x:",
//                        g_apsBindingTbl[i].dstExtAddrInfo.extAddr[ii]);
//            }
//            printf("\r\n");
//
//            irq_restore(r);
//        }
//    }

    if(zb_isDeviceJoinedNwk()){

        TL_SCHEDULE_TASK(forceReportBatVoltage, NULL);
        TL_SCHEDULE_TASK(forceReportBatPercent, NULL);
        TL_SCHEDULE_TASK(forceReportTemperature, NULL);
        TL_SCHEDULE_TASK(forceReportHumidity, NULL);

    }

    g_appCtx.timerForcedReportEvt = NULL;

    return -1;
}


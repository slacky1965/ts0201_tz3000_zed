#include "app_main.h"

static void forceReportBattery(void *args) {

    struct report_t {
        u8 numAttr;
        zclReport_t attr[2];
    };

    struct report_t report;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    report.numAttr = 0;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    if (report.numAttr) {
        zcl_sendReportAttrsCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR, ZCL_CLUSTER_GEN_POWER_CFG, (zclReportCmd_t* )&report);
    }
}


static void forceReportTemperature(void *args) {

    struct report_t {
        u8 numAttr;
        zclReport_t attr[6];
    };

    struct report_t report;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    report.numAttr = 0;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_SENSORS_READ_PERIOD);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_TEMPERATURE_OFFSET);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_TEMPERATURE_ONOFF);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_TEMPERATURE_ONOFF_LOW);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, ZCL_TEMPERATURE_MEASUREMENT_ATTRID_TEMPERATURE_ONOFF_HIGH);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    if (report.numAttr) {
        zcl_sendReportAttrsCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR, ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, (zclReportCmd_t* )&report);
    }
}

static void forceReportHumidity(void *args) {

    struct report_t {
        u8 numAttr;
        zclReport_t attr[5];
    };

    struct report_t report;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    report.numAttr = 0;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_HUMIDITY_OFFSET);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_HUMIDITY_ONOFF);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_HUMIDITY_ONOFF_LOW);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_HUMIDITY_ONOFF_HIGH);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    if (report.numAttr) {
        zcl_sendReportAttrsCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR, ZCL_CLUSTER_MS_RELATIVE_HUMIDITY, (zclReportCmd_t* )&report);
    }
}

static void forceReportSwitchAction(void *args) {

    struct report_t {
        u8 numAttr;
        zclReport_t attr[1];
    };

    struct report_t report;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    report.numAttr = 0;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, ZCL_ATTRID_SWITCH_ACTION);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    if (report.numAttr) {
        zcl_sendReportAttrsCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, (zclReportCmd_t* )&report);
    }
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

        TL_SCHEDULE_TASK(forceReportBattery, NULL);
        TL_SCHEDULE_TASK(forceReportTemperature, NULL);
        TL_SCHEDULE_TASK(forceReportHumidity, NULL);
        TL_SCHEDULE_TASK(forceReportSwitchAction, NULL);

    }

    g_appCtx.timerForcedReportEvt = NULL;

    return -1;
}


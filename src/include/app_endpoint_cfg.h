#ifndef SRC_INCLUDE_APP_ENDPOINT_CFG_H_
#define SRC_INCLUDE_APP_ENDPOINT_CFG_H_

#define APP_ENDPOINT1 0x01
#define APP_ENDPOINT2 0x02
#define APP_ENDPOINT3 0x03
#define APP_ENDPOINT4 0x04
#define APP_ENDPOINT5 0x05

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct{
    uint8_t  zclVersion;
    uint8_t  appVersion;
    uint8_t  stackVersion;
    uint8_t  hwVersion;
    uint8_t  manuName[ZCL_BASIC_MAX_LENGTH];
    uint8_t  modelId[ZCL_BASIC_MAX_LENGTH];
    uint8_t  dateCode[ZCL_BASIC_MAX_LENGTH];
    uint8_t  powerSource;
    uint8_t  genDevClass;                        //attr 8
    uint8_t  genDevType;                         //attr 9
    uint8_t  deviceEnable;
    uint8_t  swBuildId[ZCL_BASIC_MAX_LENGTH];    //attr 4000
} zcl_basicAttr_t;

/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
    uint16_t identifyTime;
}zcl_identifyAttr_t;

/**
 *  @brief Defined for power configuration cluster attributes
 */
typedef struct{
    uint8_t  batteryVoltage;      //0x20
    uint8_t  batteryPercentage;   //0x21
}zcl_powerAttr_t;

/**
 *  @brief Defined for ias zone cluster attributes
 */
typedef struct {
    uint8_t     zoneState;
    uint16_t    zoneType;
    uint16_t    zoneStatus;
    extAddr_t   iasCieAddr;
    uint8_t     zoneId;
} zcl_iasZoneAttr_t;


/**
 *  @brief Defined for tempearute cluster attributes
 */
typedef struct {
    int16_t     value;
    int16_t     minValue;
    int16_t     maxValue;
    uint16_t    read_sensors_period;            /* 10 sec - 10 ... 60           */
    int16_t     temperature_offset;             /* - 5 <-> +5 * 100             */
    uint8_t     temperature_onoff;              /* 0 - disabled, 1 - enabled    */
    int16_t     temperature_onoff_low;          /* -40 (-4000)                  */
    int16_t     temperature_onoff_high;         /* 125 (12500)                  */
} zcl_temperatureAttr_t;

/**
 *  @brief Defined for humidity cluster attributes
 */
typedef struct {
    uint16_t    value;
    uint16_t    minValue;
    uint16_t    maxValue;
    int16_t     humidity_offset;                /* - 10 <-> +10                 */
    uint8_t     humidity_onoff;                 /* 0 - disabled, 1 - enabled    */
    uint16_t    humidity_onoff_low;             /* 0                            */
    uint16_t    humidity_onoff_high;            /* 100 (10000)                  */
} zcl_humidityAttr_t;

typedef struct {
    u8 switchType;
    u8 switchActions;
} zcl_onOffSwitchCfgAttr_t;

extern const zcl_specClusterInfo_t g_appEp1ClusterList[];
extern const zcl_specClusterInfo_t g_appEp2ClusterList[];
extern const af_simple_descriptor_t app_ep1Desc;
extern const af_simple_descriptor_t app_ep2Desc;

extern uint8_t APP_EP1_CB_CLUSTER_NUM;
extern uint8_t APP_EP2_CB_CLUSTER_NUM;

#define zcl_sceneAttrGet()          &g_zcl_sceneAttrs
#define zcl_temperatureAttrGet()    &g_zcl_temperatureAttrs
#define zcl_humidityAttrGet()       &g_zcl_humidityAttrs
#define zcl_onOffSwitchCfgAttrGet() g_zcl_onOffSwitchCfgAttrs

/* Attributes */
extern zcl_basicAttr_t              g_zcl_basicAttrs;
extern zcl_identifyAttr_t           g_zcl_identifyAttrs;
extern zcl_temperatureAttr_t        g_zcl_temperatureAttrs;
extern zcl_humidityAttr_t           g_zcl_humidityAttrs;
extern zcl_onOffSwitchCfgAttr_t     g_zcl_onOffSwitchCfgAttrs[];

#endif /* SRC_INCLUDE_APP_ENDPOINT_CFG_H_ */

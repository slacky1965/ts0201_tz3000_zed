#ifndef SRC_ZCL_ZCL_RELATIVE_HUMIDITY_MEASUREMENT_H_
#define SRC_ZCL_ZCL_RELATIVE_HUMIDITY_MEASUREMENT_H_

#include "app_cfg.h"

/********************* Humidity Measurement ************************/
#if ZCL_HUMIDITY_MEASUREMENT_SUPPORT
#define ZCL_HUMIDITY_MEASUREMENT
#endif



#define ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MEASUREDVALUE         0x0000
#define ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MINMEASUREDVALUE      0x0001
#define ZCL_RELATIVE_HUMIDITY_MEASUREMENT_ATTRID_MAXMEASUREDVALUE      0x0002

status_t zcl_humidity_measurement_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


#endif /* SRC_ZCL_ZCL_RELATIVE_HUMIDITY_MEASUREMENT_H_ */

/********************************************************************************************************
 * @file    version_cfg.h
 *
 * @brief   This is the header file for version_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#ifndef SRC_INCLUDE_VERSION_CFG_H_
#define SRC_INCLUDE_VERSION_CFG_H_

#include "../common/comm_cfg.h"

#define BOARD_HXDZ_ZBWSD_V02                0x15
#define BOARD_IHSENO_IC_V0                  0x21
#define BOARD_3                             0x22
#define BOARD_4                             0x23
#define BOARD_5                             0x24

#ifndef BOARD
#define BOARD                               BOARD_HXDZ_ZBWSD_V02 //BOARD_IHSENO_IC_V0 //
#endif

#define APP_RELEASE                         0x10        //app release 1.0
#define APP_BUILD                           0x06        //app build 06, full version - v1.0.06

#define STACK_RELEASE                       0x30        //stack release 3.0
#define STACK_BUILD                         0x01        //stack build 01
#define HW_VERSION                          0x01

#ifndef BUILD_DATE
#define BUILD_DATE "20250611"
#endif

#ifndef ZCL_BASIC_DATE_CODE
#define ZCL_BASIC_DATE_CODE    BUILD_DATE
#endif

#ifndef ZCL_BASIC_LOC_DESC
    #define ZCL_BASIC_LOC_DESC     {7,'U','N','K','N','O','W','N'}
#endif
#ifndef ZCL_BASIC_BUILD_ID
    #define ZCL_BASIC_BUILD_ID     {10,'0','1','2','2','0','5','2','0','1','7'}
#endif
#ifndef ZCL_BASIC_SW_BUILD_ID //max 16 chars v1.3.02
    #define ZCL_BASIC_SW_BUILD_ID       {7,'v',(APP_RELEASE>>4)+0x30,'.',(APP_RELEASE&0xf)+0x30,'.',(APP_BUILD>>4)+0x30,(APP_BUILD&0xf)+0x30, 0}
#endif

/*
 * 0x04 - Watermeter
 * 0x07 - ElectricityMeter
 * 0x08 - Template
 * 0x09 - Remote Control
 * 0x0a - Livolo_switch_2keys
 * 0x0b - Livolo_switch_1key
 * 0x0c - Livolo_switch_1key_dimmable
 * 0x0d - Smoke_sensor
 * 0x0e - Livolo_curtain_control
 * 0x0f - Livolo_thermostat
 * 0x10 - EKF_switch_2keys_battery
 * 0x11 - Tuya Thermostat
 * 0x12 - ECM_DIN1_counter
 * 0x13 - Tuya CO2 Sensor
 * 0x14 - Air monitor
 * 0x15 - Tuya Temperature and Humidity sensors (board_hxdz_zbwsd_v02.h)
 * 0x16 - Tuya Temperature and Humidity sensors ts0601_tze200 ZG-227Z
 * 0x17 - Tuya water leak sensor ZG-222ZA, ZTU or tlsr825x with 1M
 * 0x18 - Tuya water leak sensor ZG-222ZA, tlsr825x with 512K
 * 0x19 - Tuya water leak sensor SNZB-05, tlsr8656 with 512K
 * 0x1A - ElectricityMeter_ABC
 * 0x1B - Test Remote Constol
 * 0x1C - Tuya mini relay control _TZ3000_kqvb5akv
 * 0x1D - Tuya relay QS-Zigbee-SEC02 _TZ3000_m8f3z8ju
 * 0x1E - Tuya relay QS-Zigbee-SEC01 _TZ3000_hdc8bbha
 * 0x1F - Tuya plug TS011F with monitoring
 * 0x20 - Tuya door/window sensor TS0203
 * 0x21 - Tuya Temperature and Humidity sensors (board_ihseno_ic_v0.h)
 * 0x22 - Tuya Temperature and Humidity sensors (board_ihseno_ic_v0.h)
 * 0x23 - Tuya Temperature and Humidity sensors (board_ihseno_ic_v0.h)
 * 0x24 - Tuya Temperature and Humidity sensors (board_ihseno_ic_v0.h)
 *
 */

#define IMAGE_TYPE_APP              (BOARD | (IMAGE_TYPE_BOOT_FLAG << 7))

/*********************************************************************************************
 * During OTA upgrade, the upgraded device will check the rules of the following three fields.
 * Refer to ZCL OTA specification for details.
 */
#define TELINK_MANUF_CODE           0x1141
#define SLACKY_MANUF_CODE           0x6565
#define MANUFACTURER_CODE_TELINK    SLACKY_MANUF_CODE//Custom ID
#define IMAGE_TYPE                  ((CHIP_TYPE << 8) | IMAGE_TYPE_APP)
#define FILE_VERSION                ((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)

/* Pre-compiled link configuration. */
#define IS_BOOT_LOADER_IMAGE                0
#define RESV_FOR_APP_RAM_CODE_SIZE          0
#define IMAGE_OFFSET                        APP_IMAGE_ADDR

#endif /* SRC_INCLUDE_VERSION_CFG_H_ */

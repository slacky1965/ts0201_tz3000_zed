#ifndef SRC_COMMON_BOARD_TH01_ZBEACON_H_
#define SRC_COMMON_BOARD_TH01_ZBEACON_H_

#if (BOARD == BOARD_TH01_ZBEACON)

/**********************************************************************
 * Product Information
 */
#define ZCL_BASIC_MFG_NAME  {10,'S','l','a','c','k','y','-','D','I','Y'}
#define ZCL_BASIC_MODEL_ID  {14,'T','S','0','2','0','1','-','z','2','4','-','S','l','D'}


#define I2C_DRV_USED        I2C_DRV_SOFT
#define SENSOR_USED         SENSOR_SHT40

#define VOLTAGE_DETECT_PIN  GPIO_PC5

/************************ Configure I2C for sensors ***************************
 *
 * PA0 - SDA
 * PB4 - SCL
 *
 */

#define I2C_SDA             GPIO_PA0
#define PA0_FUNC            AS_GPIO
#define PA0_INPUT_ENABLE    1
#define PA0_OUTPUT_ENABLE   0
#define PA0_DATA_OUT        0
#define PULL_WAKEUP_SRC_PA0 PM_PIN_PULLUP_10K

#define I2C_SCL             GPIO_PD4
#define PD4_FUNC            AS_GPIO
#define PD4_INPUT_ENABLE    1
#define PD4_OUTPUT_ENABLE   0
#define PD4_DATA_OUT        0
#define PULL_WAKEUP_SRC_PBD PM_PIN_PULLUP_10K

#define I2C_CLOCK           200000//200K

/**************************** Configure UART ***************************************/
#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN   GPIO_PB1
#define DEBUG_BAUDRATE      115200
#endif /* UART_PRINTF_MODE */

/********************* Configure External Battery GPIO ******************************/
#define VOLTAGE_DETECT_PIN  GPIO_PC5

/***************************** Configure LED  ***************************************/

#define LED_ON              1
#define LED_OFF             0
#define LED1                GPIO_PB6
#define PB6_FUNC            AS_GPIO
#define PB6_OUTPUT_ENABLE   ON
#define PB6_INPUT_ENABLE    OFF
#define PB6_DATA_OUT        LED_OFF

/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM      1

#define BUTTON1             GPIO_PB7
#define PB7_FUNC            AS_GPIO
#define PB7_OUTPUT_ENABLE   OFF
#define PB7_INPUT_ENABLE    ON
#define PULL_WAKEUP_SRC_PB7 PM_PIN_PULLUP_1M

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM          KB_MAP_NORMAL
#define KB_MAP_FN           KB_MAP_NORMAL

#define KB_DRIVE_PINS      {NULL }
#define KB_SCAN_PINS       {BUTTON1}

#endif /* (BOARD == BOARD_TH01_ZBEACON) */


#endif /* SRC_COMMON_BOARD_TH01_ZBEACON_H_ */

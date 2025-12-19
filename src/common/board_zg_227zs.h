#ifndef SRC_COMMON_BOARD_ZG_227ZS_H_
#define SRC_COMMON_BOARD_ZG_227ZS_H_

#if (BOARD == BOARD_ZG_227ZS)

/**********************************************************************
 * Product Information
 */
#define ZCL_BASIC_MFG_NAME  {10,'S','l','a','c','k','y','-','D','I','Y'}
#define ZCL_BASIC_MODEL_ID  {14,'T','S','0','2','0','1','-','z','2','3','-','S','l','D'}


#define I2C_DRV_USED        I2C_DRV_SOFT
#define SENSOR_USED         SENSOR_AHT20

#define VOLTAGE_DETECT_PIN  GPIO_PC5

/************************ Configure I2C for sensors ***************************
 *
 * PC3 - SDA
 * PC4 - SCL
 *
 */

#define I2C_SDA             GPIO_PC3
#define PC3_FUNC            AS_GPIO
#define PC3_INPUT_ENABLE    1
#define PC3_OUTPUT_ENABLE   0
#define PC3_DATA_OUT        0
#define PULL_WAKEUP_SRC_PC3 PM_PIN_PULLUP_10K

#define I2C_SCL             GPIO_PC4
#define PC4_FUNC            AS_GPIO
#define PC4_INPUT_ENABLE    1
#define PC4_OUTPUT_ENABLE   0
#define PC4_DATA_OUT        0
#define PULL_WAKEUP_SRC_PC4 PM_PIN_PULLUP_10K

#define I2C_CLOCK           200000//200K

/**************************** Configure UART ***************************************/
#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN   GPIO_PB1
#define DEBUG_BAUDRATE      115200
#endif /* UART_PRINTF_MODE */

/********************* Configure External Battery GPIO ******************************/
#define VOLTAGE_DETECT_PIN  GPIO_PC5

/***************************** Configure LED  ***************************************/

#define LED_ON              0
#define LED_OFF             1
#define LED1                GPIO_PC2
#define PC2_FUNC            AS_GPIO
#define PC2_OUTPUT_ENABLE   ON
#define PC2_INPUT_ENABLE    OFF
#define PC2_DATA_OUT        LED_OFF

/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM      1

#define BUTTON1             GPIO_PD4
#define PD4_FUNC            AS_GPIO
#define PD4_OUTPUT_ENABLE   OFF
#define PD4_INPUT_ENABLE    ON
#define PULL_WAKEUP_SRC_PD4 PM_PIN_PULLUP_1M

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM          KB_MAP_NORMAL
#define KB_MAP_FN           KB_MAP_NORMAL

#define KB_DRIVE_PINS      {NULL }
#define KB_SCAN_PINS       {BUTTON1}

#endif /* (BOARD == BOARD_ZG_227ZS) */


#endif /* SRC_COMMON_BOARD_ZG_227ZS_H_ */

#ifndef SRC_COMMON_BOARD_ZTH01_H_
#define SRC_COMMON_BOARD_ZTH01_H_

#if (BOARD == BOARD_ZTH01)
// _TZ3000_bgsigers

/**********************************************************************
 * Product Information
 */
#define ZCL_BASIC_MFG_NAME  {10,'S','l','a','c','k','y','-','D','I','Y'}
#define ZCL_BASIC_MODEL_ID  {14,'T','S','0','2','0','1','-','z','3','1','-','S','l','D'}


#define I2C_DRV_USED        I2C_DRV_SOFT
#define SENSOR_USED         SENSOR_SHT30

/************************ Configure I2C for sensors ***************************
 *
 * PD2 - SDA
 * PC3 - SCL
 *
 */

#define I2C_SDA             GPIO_PD2
#define PD2_FUNC            AS_GPIO
#define PD2_INPUT_ENABLE    1
#define PD2_OUTPUT_ENABLE   0
#define PD2_DATA_OUT        0
#define PULL_WAKEUP_SRC_PD2 PM_PIN_PULLUP_10K

#define I2C_SCL             GPIO_PC3
#define PC3_FUNC            AS_GPIO
#define PC3_INPUT_ENABLE    1
#define PC3_OUTPUT_ENABLE   0
#define PC3_DATA_OUT        0
#define PULL_WAKEUP_SRC_PC3 PM_PIN_PULLUP_10K

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
#define LED_STATUS          GPIO_PC2
#define PC2_FUNC            AS_GPIO
#define PC2_OUTPUT_ENABLE   ON
#define PC2_INPUT_ENABLE    OFF
#define PC2_DATA_OUT        LED_OFF

#define LED_PERMIT          LED_STATUS
#define LED1                LED_STATUS

/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM      1

#define BUTTON1             GPIO_PB4
#define PB4_FUNC            AS_GPIO
#define PB4_OUTPUT_ENABLE   OFF
#define PB4_INPUT_ENABLE    ON
#define PULL_WAKEUP_SRC_PB4 PM_PIN_PULLUP_1M

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM          KB_MAP_NORMAL
#define KB_MAP_FN           KB_MAP_NORMAL

#define KB_DRIVE_PINS      {NULL }
#define KB_SCAN_PINS       {BUTTON1}

#endif /* (BOARD == BOARD_ZTH01) */

#endif /* SRC_COMMON_BOARD_ZTH01_H_ */

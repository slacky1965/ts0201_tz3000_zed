#ifndef SRC_COMMON_BOARD_WSD_A002_H_
#define SRC_COMMON_BOARD_WSD_A002_H_

#if (BOARD == BOARD_WSD_A002)
// _TZ3000_bguser20 

/**********************************************************************
 * Product Information
 */
#define ZCL_BASIC_MFG_NAME  {10,'S','l','a','c','k','y','-','D','I','Y'}
#define ZCL_BASIC_MODEL_ID  {14,'T','S','0','2','0','1','-','z','2','9','-','S','l','D'}


#define I2C_DRV_USED        I2C_DRV_HARD
#define SENSOR_USED         SENSOR_CHT8305

/************************ Configure I2C for sensors ***************************
 *
 * PC2 - SDA
 * PC3 - SCL
 *
 */

#define I2C_PIN_GROUP       I2C_GPIO_GROUP_C2C3
#define I2C_CLOCK           200000//200K

/**************************** Configure UART ***************************************/
#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN   GPIO_SWS
//#define DEBUG_INFO_TX_PIN   GPIO_PB1
#define DEBUG_BAUDRATE      115200
#endif /* UART_PRINTF_MODE */

/********************* Configure External Battery GPIO ******************************/
#define VOLTAGE_DETECT_PIN      GPIO_PC5

/***************************** Configure LED  ***************************************/

#define LED_ON              1
#define LED_OFF             0
#define LED_STATUS          GPIO_PD2
#define PD2_FUNC            AS_GPIO
#define PD2_OUTPUT_ENABLE   ON
#define PD2_INPUT_ENABLE    OFF
#define PD2_DATA_OUT        LED_OFF

#define LED_PERMIT          LED_STATUS
#define LED1                LED_STATUS

/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM      1

#define BUTTON1             GPIO_PB5
#define PB5_FUNC            AS_GPIO
#define PB5_OUTPUT_ENABLE   OFF
#define PB5_INPUT_ENABLE    ON
#define PULL_WAKEUP_SRC_PB5 PM_PIN_PULLUP_1M

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM          KB_MAP_NORMAL
#define KB_MAP_FN           KB_MAP_NORMAL

#define KB_DRIVE_PINS      {NULL }
#define KB_SCAN_PINS       {BUTTON1}

#endif /* (BOARD == BOARD_WSD_A002) */

#endif /* SRC_COMMON_BOARD_WSD_A002_H_ */

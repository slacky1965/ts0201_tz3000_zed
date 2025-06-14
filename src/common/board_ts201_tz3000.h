#ifndef SRC_COMMON_BOARD_TS201_TZ3000_H_
#define SRC_COMMON_BOARD_TS201_TZ3000_H_

#define VOLTAGE_DETECT_PIN      GPIO_PC5

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
#define DEBUG_INFO_TX_PIN       GPIO_PB1
#define DEBUG_BAUDRATE          115200
#endif /* UART_PRINTF_MODE */

/********************* Configure External Battery GPIO ******************************/
#define VOLTAGE_DETECT_PIN      GPIO_PC5

/***************************** Configure LED  ***************************************/

#define LED_ON                  1
#define LED_OFF                 0
#define LED_STATUS              GPIO_PB4
#define PB4_FUNC                AS_GPIO
#define PB4_OUTPUT_ENABLE       ON
#define PB4_INPUT_ENABLE        OFF
#define PB4_DATA_OUT            LED_OFF

#define LED_POWER               GPIO_PA0
#define PA0_FUNC                AS_GPIO
#define PA0_OUTPUT_ENABLE       ON
#define PA0_INPUT_ENABLE        OFF

#define LED_PERMIT              LED_STATUS
#define LED1                    LED_STATUS


/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM  1

#define BUTTON1                 GPIO_PC0
#define PC0_FUNC                AS_GPIO
#define PC0_OUTPUT_ENABLE       OFF
#define PC0_INPUT_ENABLE        ON
#define PULL_WAKEUP_SRC_PC0     PM_PIN_PULLUP_1M

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1}



#endif /* SRC_COMMON_BOARD_TS201_TZ3000_H_ */

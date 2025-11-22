#ifndef SRC_INCLUDE_APP_PM_H_
#define SRC_INCLUDE_APP_PM_H_

#define APP_ANALOG_REG_SEC_LOW      DEEP_ANA_REG1   // analog register
#define APP_ANALOG_REG_SEC_HIGH     DEEP_ANA_REG7   // analog register
#define APP_ANALOG_REG_DEEP_SLEEP   DEEP_ANA_REG8   // analog register
#define APP_ANALOG_REG_SW_ONOFF     DEEP_ANA_REG9   // analog register

typedef union {
    int16_t time;
    uint8_t a_time[2];
} analog_reg_time_t;

//#define TIME_LONG_DEEP_SLEEP    3600            // Time of long deep sleep in sec. 3600

extern analog_reg_time_t analog_reg_time;
extern uint32_t last_timer_duration;


void app_pm_lowPowerEnter();

#endif /* SRC_INCLUDE_APP_PM_H_ */

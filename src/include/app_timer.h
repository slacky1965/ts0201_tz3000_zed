#ifndef SRC_INCLUDE_APP_TIMER_H_
#define SRC_INCLUDE_APP_TIMER_H_

#define APP_TIMER_PERIOD	TIMEOUT_2HOUR

typedef struct {
    ev_timer_event_t *timerAPPEvt;
    uint32_t timeout;

} app_timer_t;

extern app_timer_t app_timer;

void app_timer_init();
bool app_timer_exceed(uint32_t ref, uint32_t ms);
uint32_t app_timeout_get();

#endif /* SRC_INCLUDE_APP_TIMER_H_ */

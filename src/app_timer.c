#include "app_main.h"

app_timer_t app_timer = {
		.timerAPPEvt = NULL
};

/*********************************************************************
 * @fn      app_timerCb
 *
 * @brief   Timer callback for the application periodic timer
 *
 * @param   args - unused callback argument
 *
 * @return  0 always
 */
static int32_t app_timerCb(void *args) {

	return 0;
}

/*********************************************************************
 * @fn      app_timer_exceed
 *
 * @brief   Check if a given time reference has exceeded a timeout in milliseconds
 *
 * @param   ref - reference tick value to compare against
 * @param   ms  - timeout duration in milliseconds
 *
 * @return  true if elapsed time >= ms, false otherwise
 */
bool app_timer_exceed(uint32_t ref, uint32_t ms) {
//    uint32_t t = app_timer.timerAPPEvt->timeout;
//    int32_t tt;
//    if (ref && t > ref) {
//        tt = (ref - t) * -1;
//        tt = (tt - app_timer.timerAPPEvt->period) * -1;
//    } else tt = ref - t;
//    return ((uint32_t)tt > ms);

    uint32_t t = app_timer.timerAPPEvt->timeout;
    uint32_t elapsed;
    if (ref && t > ref) {
        elapsed = ref + app_timer.timerAPPEvt->period - t;
    } else {
        elapsed = ref - t;
    }
//    APP_DEBUG(DEBUG_APP_TIMER_EN, "exceed: ref=%d t=%d ms=%d elapsed=%d\r\n", ref, t, ms, elapsed);
    return elapsed >= ms;
}

/*********************************************************************
 * @fn      app_timeout_get
 *
 * @brief   Get the current timeout tick value from the application timer
 *
 * @param   None
 *
 * @return  Current timeout tick value
 */
uint32_t app_timeout_get(void) {
	return app_timer.timerAPPEvt->timeout;
}

/*********************************************************************
 * @fn      app_timer_init
 *
 * @brief   Initialize the application periodic timer
 *
 * @param   None
 *
 * @return  None
 */
void app_timer_init(void) {
	if (!app_timer.timerAPPEvt) {
		app_timer.timerAPPEvt = TL_ZB_TIMER_SCHEDULE(app_timerCb, NULL, APP_TIMER_PERIOD);

//		 printf("repiod: %d, timeout: %d\r\n", app_timer.timerAPPEvt->period, app_timer.timerAPPEvt->timeout);

	}
}

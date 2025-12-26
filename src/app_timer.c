#include "app_main.h"

app_timer_t app_timer = {
		.timerAPPEvt = NULL
};

static int32_t app_timerCb(void *args) {

	return 0;
}

bool app_timer_exceed(uint32_t ref, uint32_t ms) {
	uint32_t t = app_timer.timerAPPEvt->timeout;
	int32_t tt;
    if (ref && t > ref) {
        tt = (ref - t) * -1;
        tt = (tt - app_timer.timerAPPEvt->period) * -1;
    } else tt = ref - t;

//    printf("tt: %d, t: %d, ref: %d, ms: %d, (tt > ms): %d\r\n", tt, t, ref, ms, ((uint32_t)tt > ms));

	return ((uint32_t)tt > ms);
}

uint32_t app_timeout_get() {
	return app_timer.timerAPPEvt->timeout;
}

void app_timer_init() {
	if (!app_timer.timerAPPEvt) {
		app_timer.timerAPPEvt = TL_ZB_TIMER_SCHEDULE(app_timerCb, NULL, APP_TIMER_PERIOD);

//		 printf("repiod: %d, timeout: %d\r\n", app_timer.timerAPPEvt->period, app_timer.timerAPPEvt->timeout);

	}
}

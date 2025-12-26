#ifndef SRC_INCLUDE_APP_BINDING_H_
#define SRC_INCLUDE_APP_BINDING_H_

typedef struct {
    aps_binding_entry_t *bind_entry;
    uint16_t short_addr;
} bind_onoff_t;

typedef struct {
    bool one_device;
    bool idle;
    uint8_t idx;
    bool addrCrdnrSet;
    addrExt_t extAddrCoordinator;
    bind_onoff_t bind_onoff[APS_BINDING_TABLE_NUM];
} app_bind_tbl_t;

int32_t app_bindTimerCb(void *args);
void app_update_bind_tbl(void *args);
bool app_edle_bind_tbl();
bool onoff_get_one_device();

#endif /* SRC_INCLUDE_APP_BINDING_H_ */

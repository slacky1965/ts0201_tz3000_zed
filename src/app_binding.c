#include "app_main.h"

static app_bind_tbl_t app_bind_tbl = {
        .one_device = false,
        .idle = true,
        .idx = 0,
        .extAddrCoordinator = {0}
};

static void onoff_cmp_addr() {

    aps_binding_entry_t *bind_entry_curr = NULL, *bind_entry_next = NULL;

    app_bind_tbl.one_device = false;

    for (uint8_t i = 0; i < app_bind_tbl.idx; i++) {
        bind_entry_curr = app_bind_tbl.bind_onoff[i].bind_entry;
        for (uint8_t idx = 0; idx < app_bind_tbl.idx; idx++) {
            bind_entry_next = app_bind_tbl.bind_onoff[idx].bind_entry;
            if (!ZB_IEEE_ADDR_CMP(bind_entry_curr->dstExtAddrInfo.extAddr, app_bind_tbl.extAddrCoordinator) && i != idx) {
                if (ZB_IEEE_ADDR_CMP(bind_entry_curr->dstExtAddrInfo.extAddr, bind_entry_next->dstExtAddrInfo.extAddr)) {
                    if (bind_entry_curr->dstExtAddrInfo.dstEp == bind_entry_next->dstExtAddrInfo.dstEp) {
                        app_bind_tbl.one_device = true;
                    }
                }
            }
        }
    }

    app_bind_tbl.idle = false;
}

static void onoff_nwkAddrReqCb(void *arg) {
    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
    zdo_nwk_addr_resp_t *rsp = (zdo_nwk_addr_resp_t *)p->zpdu;

    if (rsp->status == ZDO_SUCCESS) {

        if (p->src_addr == 0x0000) {
            ZB_IEEE_ADDR_COPY(app_bind_tbl.extAddrCoordinator, rsp->ieee_addr_remote);
        }
    }

    onoff_cmp_addr();
}

static void onoff_nwkAddrReq(addrExt_t extAddr) {
    u8 sn = 0;
    zdo_nwk_addr_req_t req;
    ZB_IEEE_ADDR_COPY(req.ieee_addr_interest, extAddr);
    req.req_type = ZDO_ADDR_REQ_SINGLE_RESP;
    req.start_index = 0;
    zb_zdoNwkAddrReq(NWK_BROADCAST_ROUTER_COORDINATOR, &req, &sn, onoff_nwkAddrReqCb);
}

void app_update_bind_tbl(void *args) {

    uint8_t i;
    uint16_t summ_addr = 0;
    aps_binding_entry_t *bind_entry = NULL;
    aps_binding_entry_t *bind_tbl = bindTblEntryGet();
    app_bind_tbl.idle = true;

    for (i = 0, app_bind_tbl.idx = 0; i < APS_BINDING_TABLE_NUM; i++) {
        if (bind_tbl->used) {
            if (bind_tbl->clusterId == ZCL_CLUSTER_GEN_ON_OFF) {
                app_bind_tbl.bind_onoff[app_bind_tbl.idx++].bind_entry = bind_tbl;
            }
        }
        bind_tbl++;
    }

    for(i = 0; i < EXT_ADDR_LEN; i++) {
        summ_addr += app_bind_tbl.extAddrCoordinator[i];
    }

    if (!summ_addr) {
        for (i = 0; i < app_bind_tbl.idx; i++) {
            bind_entry = app_bind_tbl.bind_onoff[i].bind_entry;
            if (bind_entry) {
                onoff_nwkAddrReq(bind_entry->dstExtAddrInfo.extAddr);
            }
        }
    } else {
        onoff_cmp_addr();
    }
}

bool onoff_get_one_device() {
    return app_bind_tbl.one_device;
}

bool app_edle_bind_tbl() {
    return app_bind_tbl.idle;
}

int32_t app_bindTimerCb(void *args) {

    if (zb_isDeviceJoinedNwk()) {
        TL_SCHEDULE_TASK(app_update_bind_tbl, NULL);
        g_appCtx.timerAppBindEvt = NULL;
        return -1;
    }

    return 0;
}

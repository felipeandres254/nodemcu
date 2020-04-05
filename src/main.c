#include "mgos.h"
#include "mgos_cron.h"
#include "mgos_wifi.h"
#include "mgos_aws_shadow.h"

#define ONBOARD_LED 2        // NodeMCU

mgos_timer_id _blink;
static void _blink_loop(void *arg) {
    mgos_gpio_toggle(ONBOARD_LED);
    (void)arg;
}
static void _blink_end(void *arg) {
    mgos_clear_timer(_blink);
    mgos_gpio_write(ONBOARD_LED, 1);
    (void)arg;
}

char DEVICE_STATUS[10];
static void shadow_update() {
    mgos_gpio_write(ONBOARD_LED, 0);

    char ssid[256]; sprintf(ssid, "null");
    if (strcmp(DEVICE_STATUS, "ONLINE") == 0 && mgos_wifi_get_connected_ssid() != NULL) {
        sprintf(ssid, "\"%s\"", mgos_wifi_get_connected_ssid());
    }
    mgos_aws_shadow_updatef(0, "{status:\"%s\",system:{uptime:%d,ram:\"%lu/%lu\",fs:\"%lu/%lu\"},wifi:{ssid:%s}}",
        DEVICE_STATUS, (int)mgos_uptime(),
        (unsigned long)mgos_get_heap_size() - (unsigned long)mgos_get_free_heap_size(),
        (unsigned long)mgos_get_heap_size(),
        (unsigned long)mgos_vfs_get_space_total("/") - (unsigned long)mgos_vfs_get_space_free("/"),
        (unsigned long)mgos_vfs_get_space_total("/"),
        ssid);

    mgos_gpio_write(ONBOARD_LED, 1);
}

static void _events_cb(int event, void *data, void *arg) {
    switch (event) {
        case MGOS_EVENT_REBOOT:
            sprintf(DEVICE_STATUS, "REBOOTING"); shadow_update();
            mgos_clear_timer(_blink);
            _blink = mgos_set_timer(100, MGOS_TIMER_REPEAT, _blink_loop, NULL);
            break;
        case MGOS_NET_EV_DISCONNECTED:
        case MGOS_EVENT_CLOUD_DISCONNECTED:
        case MGOS_WIFI_EV_STA_DISCONNECTED:
            sprintf(DEVICE_STATUS, "OFFLINE"); shadow_update();
            mgos_clear_timer(_blink);
            _blink = mgos_set_timer(100, MGOS_TIMER_REPEAT, _blink_loop, NULL);
            break;
        case MGOS_NET_EV_CONNECTED:
        case MGOS_EVENT_CLOUD_CONNECTED:
        case MGOS_WIFI_EV_STA_CONNECTED:
        case MGOS_WIFI_EV_STA_IP_ACQUIRED:
            mgos_set_timer(100, 0, _blink_end, NULL);
            sprintf(DEVICE_STATUS, "ONLINE"); shadow_update();
            break;
    }
    (void)data;
    (void)arg;
}

static void _crontab_cb(void *data, mgos_cron_id_t id) {
    shadow_update();
    (void)data;
    (void)id;
}

enum mgos_app_init_result mgos_app_init(void) {
    mgos_gpio_setup_output(ONBOARD_LED, 1);
    _blink = mgos_set_timer(100, MGOS_TIMER_REPEAT, _blink_loop, NULL);

    // System events
    mgos_event_add_handler(MGOS_EVENT_REBOOT, _events_cb, NULL);
    mgos_event_add_group_handler(MGOS_EVENT_GRP_NET, _events_cb, NULL);
    mgos_event_add_handler(MGOS_EVENT_CLOUD_CONNECTED, _events_cb, NULL);
    mgos_event_add_handler(MGOS_EVENT_CLOUD_DISCONNECTED, _events_cb, NULL);
    mgos_event_add_group_handler(MGOS_WIFI_EV_BASE, _events_cb, NULL);

    // Crontab events
    mgos_cron_add("0 */5 * * * *", _crontab_cb, NULL);

    mgos_set_timer(1000, 0, _blink_end, NULL);
    return MGOS_APP_INIT_SUCCESS;
}

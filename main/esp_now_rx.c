#include "esp_now_rx.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_log.h"
#include <string.h>

static drone_recv_cb_t user_callback = NULL;

static void internal_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {

    if (len == sizeof(RC_Data_t) && user_callback != NULL) {

        int rssi = recv_info->rx_ctrl->rssi;

        user_callback((RC_Data_t *)data, rssi);
    }
}

void esp_now_rx_init(drone_recv_cb_t callback) {
    user_callback = callback;

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(internal_recv_cb));
}
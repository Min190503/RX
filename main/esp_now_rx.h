#ifndef ESP_NOW_RX_H
#define ESP_NOW_RX_H

#include "mamba_uart.h" 


typedef void (*drone_recv_cb_t)(RC_Data_t *data, int rssi);

void esp_now_rx_init(drone_recv_cb_t callback);

#endif
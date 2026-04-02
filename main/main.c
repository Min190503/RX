#include <stdio.h>
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"     
#include "esp_now_rx.h"
#include "mamba_uart.h"
#include <string.h>
#include "esp_now.h"


//struct telemetry
#pragma pack(push, 1)
typedef struct 
{
    int rssi;
    uint32_t pps;
    float volt;
} Telemetry_Packet_t;
#pragma pack(pop)

//mac tay cam 
uint8_t transmitter_mac[] = {0x80, 0xB5, 0x4E, 0xC1, 0xDE, 0x88 };
uint32_t last_rc_time = 0;

RC_Data_t last_data = {0}; 
volatile uint32_t packet_count = 0; 
volatile int current_rssi = 0;

void on_controller_data_received(RC_Data_t *incoming_data, int rssi) {
    last_data = *incoming_data;
    current_rssi = rssi;
    packet_count++; 

    mamba_uart_send(incoming_data);
}

void app_main(void) {
    printf("Khoi đong he thong Drone...\n");
    mamba_uart_init();
    esp_now_rx_init(on_controller_data_received);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, transmitter_mac, 6);
    peerInfo.channel = 1;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
    printf("Drone da san sang! Cho ket noi tu tay cam...\n");
    
    while(1) {
        uint32_t pps = __atomic_exchange_n(&packet_count, 0, __ATOMIC_RELAXED);

        printf("Song(RSSI): %4d dBm | Toc do nhan: %3lu goi/giay\n", current_rssi, pps);

        Telemetry_Packet_t tele;
        tele.rssi = current_rssi;
        tele.pps = pps;
        tele.volt = 12.6; //test

        esp_now_send(transmitter_mac, (uint8_t *)&tele, sizeof(Telemetry_Packet_t));
               
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
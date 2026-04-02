#include "mamba_uart.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_NUM         UART_NUM_1
#define UART_TX_PIN      17  // Nối RX Mamba
#define UART_RX_PIN      18  // Nối TX Mamba
#define UART_BAUDRATE    115200

// Struct đóng gói gửi Mamba
typedef struct {
    uint8_t  header;
    uint16_t throttle;
    uint16_t roll;
    uint16_t pitch;
    uint16_t yaw;
    uint8_t  sw1;
    uint8_t  sw2;
    uint8_t  checksum;
} __attribute__((packed)) Mamba_Packet_t;

void mamba_uart_init(void) {
    uart_config_t uart_config = {
        .baud_rate = UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);
}

void mamba_uart_send(RC_Data_t *data) {
    Mamba_Packet_t pkt;
    pkt.header = 0xAA;
    pkt.throttle = data->throttle;
    pkt.roll = data->roll;
    pkt.pitch = data->pitch;
    pkt.yaw = data->yaw;
    pkt.sw1 = data->sw1;
    pkt.sw2 = data->sw2;

    uint8_t cs = 0;
    uint8_t *p = (uint8_t*)&pkt;
    for(int i = 0; i < sizeof(Mamba_Packet_t) - 1; i++) {
        cs ^= p[i];
    }
    pkt.checksum = cs;

    uart_write_bytes(UART_NUM, (const char*)&pkt, sizeof(Mamba_Packet_t));
}
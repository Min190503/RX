#ifndef MAMBA_UART_H
#define MAMBA_UART_H

#include <stdint.h>

#pragma pack(push, 1) 

typedef struct {
    uint16_t throttle;
    uint16_t roll;
    uint16_t pitch;
    uint16_t yaw;
    uint8_t  sw1;
    uint8_t  sw2;
} RC_Data_t;

#pragma pack(pop) 

void mamba_uart_init(void);
void mamba_uart_send(RC_Data_t *data);

#endif
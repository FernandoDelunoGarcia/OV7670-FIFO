#ifndef INCLUDES_OV7670_H_
#define INCLUDES_OV7670_H_

#include <stdint.h>

int OV7670_Init(void);
void OV7670_Setup_QVGA_RGB565(void);
void OV7670_WriteRegister(uint8_t addr, uint8_t data);
uint8_t OV7670_ReadRegister(uint8_t addr);

#endif

/*
 * HardwareProxy.h
 *
 *  Created on: Sep 29, 2017
 *      Author: Fernando
 */

#ifndef INCLUDES_HARDWAREPROXY_H_
#define INCLUDES_HARDWAREPROXY_H_

#include <stdint.h>

int Hw_Init(void);
int Hw_SendByte(uint8_t data);
int Hw_GetByte(uint8_t * data);
void Hw_Delay(uint32_t dlyTicks);

void Hw_WEN_DIS();
void Hw_WEN_EN();
void Hw_WRST_DIS();
void Hw_WRST_EN();
void Hw_RRST_DIS();
void Hw_RRST_EN();
void Hw_OE_DIS();
void Hw_OE_EN();
void Hw_RCLK_HIGH();
void Hw_RCLK_LOW();
uint8_t Hw_READ_BYTE();
void Hw_SCL_HIGH(void);
void Hw_SCL_LOW();
void Hw_SCL_OUTPUT();
void Hw_SDA_HIGH();
void Hw_SDA_LOW();
void Hw_SDA_INPUT();
void Hw_SDA_OUTPUT();
uint32_t Hw_READ_SDA();
uint8_t Hw_READ_VSYNC();

#endif /* INCLUDES_HARDWAREPROXY_H_ */

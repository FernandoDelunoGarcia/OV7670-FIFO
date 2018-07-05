/*
 * Application.h
 *
 *  Created on: Jun 18, 2017
 *      Author: Fernando
 */

#ifndef INCLUDES_APPLICATION_H_
#define INCLUDES_APPLICATION_H_

#include <stdint.h>

void App_Init(void);
int App_SerialWrite(uint8_t * buffer, uint32_t size);
int App_SerialRead(uint8_t * byteReceived);
int App_Delay(uint32_t delay_ms);

#endif /* INCLUDES_APPLICATION_H_ */

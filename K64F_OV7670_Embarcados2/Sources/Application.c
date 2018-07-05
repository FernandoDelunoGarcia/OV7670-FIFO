/*
 * Appliance.c
 *
 *  Created on: Sep 21, 2017
 *      Author: Fernando
 */

#include <stdlib.h>
#include "Application.h"
#include "HardwareProxy.h"

void App_Init(void)
{
	Hw_Init();
}

int App_SerialWrite(uint8_t * buffer, uint32_t size)
{
	if((buffer != NULL) && (size > 0)){

		while (size--){
			Hw_SendByte(*buffer++);
		}

		return 0;
	}

	return -1;
}

int App_SerialRead(uint8_t * byteReceived)
{
	if(byteReceived != NULL){
		return Hw_GetByte(byteReceived);
	}

	return -1;
}

int App_Delay(uint32_t delay_ms)
{
	Hw_Delay(delay_ms);
}

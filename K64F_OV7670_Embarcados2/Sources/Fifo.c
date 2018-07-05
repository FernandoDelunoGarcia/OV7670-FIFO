/*
 * Camera.c
 *
 *  Created on: Oct 11, 2017
 *      Author: Fernando
 */

#include "Fifo.h"
#include "HardwareProxy.h"


void FIFO_Init(void)
{
	//Startup condition of control signals
    Hw_RCLK_LOW();
    Hw_WEN_DIS();
	Hw_RRST_EN();
	Hw_WRST_EN();
	Hw_OE_EN();
}

void FIFO_StartCapture(void)
{
	//Waits for the falling edge of the VSYNC signal
	while(Hw_READ_VSYNC());

	//The frame capture starts here.
	//First, resets the fifo write pointer
	Hw_WRST_DIS();

	//Enables the write signal allowing the data to be written into the fifo
	Hw_WEN_EN();

	//Now waits for the end of frame
	while(!Hw_READ_VSYNC());
	while(Hw_READ_VSYNC());

	//Disables write signal
	Hw_WEN_DIS();

	//Enables read_reset signal
	Hw_RRST_EN();

	//Enables write_reset signal
	Hw_WRST_EN();

	//Generates a clock pulse
	Hw_Delay(1);
	Hw_RCLK_LOW();
	Hw_Delay(1);
	Hw_RCLK_HIGH();
	Hw_Delay(1);

	//Disables read_reset signal
	Hw_RRST_DIS();
}

uint16_t FIFO_GetPixel16(void)
{
	uint16_t result = 0;

	//Gets the first byte from fifo buffer
	Hw_RCLK_HIGH();					//Turns on the clock line
	result = (Hw_READ_BYTE() << 8);	//reads one byte
	Hw_RCLK_LOW(); 					//Turns off the clock line

	//Gets the second byte from fifo buffer
	Hw_RCLK_HIGH(); 			//Turns on the clock line
	result |= Hw_READ_BYTE();	//reads one byte
	Hw_RCLK_LOW(); 				//Turns off the clock line

	return result;
}

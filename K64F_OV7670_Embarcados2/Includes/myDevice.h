/*
 * myDevice.h
 *
 *  Created on: Mar 26, 2018
 *      Author: Fernando
 */

#ifndef INCLUDES_MYDEVICE_H_
#define INCLUDES_MYDEVICE_H_

#include "MK64F12.h"

/**** Interface Camera VGA ****/

#define VSYNC_PIN 3
#define WEN_PIN 1
#define WRST_PIN 2
#define RRST_PIN 1
#define OE_PIN 24
#define RCLK_PIN 25

__INLINE void Hw_WEN_DIS()
{
	GPIOD_PCOR = (1 << WEN_PIN);
}

__INLINE void Hw_WEN_EN()
{
	GPIOD_PSOR = (1 << WEN_PIN);
}

__INLINE void Hw_WRST_DIS()
{
	GPIOA_PSOR = (1 << WRST_PIN);
}

__INLINE void Hw_WRST_EN()
{
	GPIOA_PCOR = (1 << WRST_PIN);
}

__INLINE void Hw_RRST_DIS()
{
	GPIOA_PSOR = (1 << RRST_PIN);
}

__INLINE void Hw_RRST_EN()
{
	GPIOA_PCOR = (1 << RRST_PIN);
}

__INLINE void Hw_OE_DIS()
{
	GPIOE_PSOR = (1 << OE_PIN);
}

__INLINE void Hw_OE_EN()
{
	GPIOE_PCOR = (1 << OE_PIN);
}

__INLINE void Hw_RCLK_HIGH()
{
	GPIOE_PSOR = (1 << RCLK_PIN);
}

__INLINE void Hw_RCLK_LOW()
{
	GPIOE_PCOR = (1 << RCLK_PIN);
}

__INLINE uint8_t Hw_READ_VSYNC()
{
	return ((GPIOD_PDIR & (1 << VSYNC_PIN)) != 0);
}

#define PC_MASK (((1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)))
#define PB_MASK ((1 << 18) | (1 << 19))

__INLINE uint8_t Hw_READ_BYTE()
{
	int portc_in;
	int portb_in;
	uint8_t byte;

	portc_in = GPIOC_PDIR & PC_MASK;
	portb_in = GPIOB_PDIR & PB_MASK;
	return (uint8_t)((portb_in >> 12) | portc_in);
}


/**** Interface SCCB ****/
#define SCL_PIN 0
#define SDA_PIN 2

__INLINE void Hw_SCL_HIGH(void)
{
	GPIOD_PSOR = (1 << SCL_PIN);
}

__INLINE void Hw_SCL_LOW()
{
	GPIOD_PCOR = (1 << SCL_PIN);
}

__INLINE void Hw_SCL_OUTPUT()
{
	GPIOD_PDDR |= (1 << SCL_PIN);
}

__INLINE void Hw_SDA_HIGH()
{
	GPIOD_PDDR &= ~(1 << SDA_PIN);
	GPIOD_PSOR = (1 << SDA_PIN);
}

__INLINE void Hw_SDA_LOW()
{
	GPIOD_PDDR |= (1 << SDA_PIN);
	GPIOD_PCOR = (1 << SDA_PIN);
}

__INLINE void Hw_SDA_INPUT()
{
	GPIOD_PDDR &= ~(1 << SDA_PIN);
	GPIOD_PSOR = (1 << SDA_PIN);
}

__INLINE void Hw_SDA_OUTPUT()
{
	GPIOD_PDDR |= (1 << SDA_PIN);
	GPIOD_PCOR = (1 << SDA_PIN);
}

__INLINE uint32_t Hw_READ_SDA(){
	return ((GPIOD_PDIR & (1 << SDA_PIN)) != 0);
}

#endif /* INCLUDES_MYDEVICE_H_ */

/*
 * OV7670.c
 *
 *  Created on: Jun 18, 2017
 *      Author: Fernando
 */

#include "OV7670.h"
#include "OV7670_Reg.h"
#include "SCCB.h"

int OV7670_Init(void)
{
	uint8_t tmp;

	SCCB_Init();

	/*Verifies PID and Version*/

    tmp = OV7670_ReadRegister(REG_PID);

    if(0x76 == tmp)
    {
    	tmp = OV7670_ReadRegister(REG_VER);

    	if(0x73 == tmp)
    	{
    		//Resets camera to default configuration
    		OV7670_WriteRegister(REG_COM7, 0x80);

    		//default value
    		OV7670_WriteRegister(REG_CLKRC, 0x80);

    		//output sequence: UYVY *** use with register COM13[1]
			OV7670_WriteRegister(REG_TSLB, TSLB_YLAST);

			//VSYNC Negative
			OV7670_WriteRegister(REG_COM10, COM10_VS_NEG);

			//White balance enable
			OV7670_WriteRegister(0x13, 0xe7);
			OV7670_WriteRegister(0x6f, 0x9f);
    		return 0;
    	}
    }

    return -1;
}

void OV7670_Setup_QVGA_RGB565(void)
{
	OV7670_WriteRegister(REG_COM7, COM7_RGB | COM7_QVGA); //QVGA and RGB format
	OV7670_WriteRegister(REG_COM15, COM15_RGB565 ); //RGB565 output

	//frame control: from linux sources
	OV7670_WriteRegister(REG_HSTART,0x15);
	OV7670_WriteRegister(REG_HSTOP,	0x03);
	OV7670_WriteRegister(REG_HREF, 0x00);
	OV7670_WriteRegister(REG_VSTART, 0x03);
	OV7670_WriteRegister(REG_VSTOP, 0x7B);
	OV7670_WriteRegister(REG_VREF, 0x00);


	//Scale mode enabled because COM7 is configured with pre-defined format (COM7_RGB)
	//then COM14_QVGA must be set
	OV7670_WriteRegister(REG_COM14, COM14_QVGA);
	OV7670_WriteRegister(REG_COM3, COM3_QVGA | COM3_DCWEN);


	//OV7670_WriteRegister(REG_MVFP, MVFP_MIRROR); //Horizontal mirror

	//datasheet: Table 2.2 Resolution Register Settings
	OV7670_WriteRegister(REG_SCALING_DCWCTR, SCALING_DCWCTR_QVGA); //vertical/horizontal down sample by 2
	OV7670_WriteRegister(REG_SCALING_PCLK_DELAY, SCALING_PCLK_DELAY_QVGA); //clock divider: divided by 2
	OV7670_WriteRegister(REG_SCALING_PCLK_DIV, SCALING_PCLK_DIV_QVGA);
	OV7670_WriteRegister(REG_SCALING_XSC, SCALING_XSC_QVGA); //horizontal scale factor
	OV7670_WriteRegister(REG_SCALING_YSC, SCALING_YSC_QVGA); //vertical scale factor

	OV7670_WriteRegister(0xb0, 0x84);

	//Saturation matrix: from linux sources
	OV7670_WriteRegister( 0x4f, 0xb3 );
	OV7670_WriteRegister( 0x50, 0xb3 );
	OV7670_WriteRegister( 0x51, 0    );
	OV7670_WriteRegister( 0x52, 0x3d );
	OV7670_WriteRegister( 0x53, 0xa7 );
	OV7670_WriteRegister( 0x54, 0xe4 );
}


uint8_t OV7670_ReadRegister(uint8_t addr)
{
    uint8_t result;

    //Start of transmission
	SCCB_Start();

	//ID Address + Sub-address
	SCCB_WriteByte(SCCB_ADDR_WR);
	SCCB_DELAY();
	SCCB_WriteByte((uint8_t)addr);

	//End of transmission
	SCCB_Stop();


	SCCB_DELAY();

	//Start of transmission
	SCCB_Start();

	//ID Address + Read Data
	SCCB_WriteByte(SCCB_ADDR_WR | 0x01);
	SCCB_ReadByte(&result);

	//End of transmission
	SCCB_Stop();

    return result;
}

void OV7670_WriteRegister(uint8_t addr, uint8_t data)
{
	//Start of transmission
	SCCB_Start();

	SCCB_WriteByte(SCCB_ADDR_WR);
	SCCB_WriteByte((uint8_t)addr);
	SCCB_WriteByte((uint8_t)data);

	//End of transmission
	SCCB_Stop();
}

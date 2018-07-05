#include <stdlib.h>
#include "SCCB.h"
#include "HardwareProxy.h"


void SCCB_Init(void)
{
	//Configures SDA and SCL as output pins and determines the initial condition
	Hw_SDA_OUTPUT();
	Hw_SCL_OUTPUT();
	Hw_SDA_HIGH();
	Hw_SCL_HIGH();

}

void SCCB_Start(void)
{
	//First, turns on the SDA and SCL lines
	Hw_SDA_HIGH();
	Hw_SCL_HIGH();

	//Then, turns off the SDA line
	SCCB_DELAY();
	Hw_SDA_LOW();

	//Finally, turns off the SCL line
	SCCB_DELAY();
	Hw_SCL_LOW();
	SCCB_DELAY();
}

void SCCB_Stop(void)
{
	//First, turns off the SDA and SCL lines
	Hw_SDA_LOW();
	Hw_SCL_LOW();

	//Then, turns on the SCL line
	SCCB_DELAY();
	Hw_SCL_HIGH();

	//Finally, turns on the SDA line
	SCCB_DELAY();
	Hw_SDA_HIGH();
}


void SCCB_WriteByte(uint8_t data)
{
    uint8_t i;

    //Performs left shift on data variable
    //Checks the MSB and then changes the SDA line to the corresponding value
	for(i=0; i<8; i++)
	{
		//Activates or deactivates the SDA line
		if((data & 0x80) != 0)
		{
			Hw_SDA_HIGH();
		}
		else
		{
			Hw_SDA_LOW();
		}

		//Generates a clock pulse
		SCCB_DELAY();
		Hw_SCL_HIGH();
		SCCB_DELAY();
		Hw_SCL_LOW();
		SCCB_DELAY();

		//shifts left one bit
		data = data << 1;
	}

	Hw_SDA_INPUT();

	//Generates a clock pulse
	SCCB_DELAY();
	Hw_SCL_HIGH();
	SCCB_DELAY();
	Hw_SCL_LOW();
}

void SCCB_ReadByte(uint8_t * data)
{
	int8_t i = 7;
    uint8_t byte=0;

	Hw_SDA_INPUT();

	//Gets 8 bits
	do
	{
		//Generates a clock pulse
		SCCB_DELAY();
		Hw_SCL_HIGH();
		SCCB_DELAY();

		//Captures SDA level
		if(Hw_READ_SDA())
		{
			byte = byte | (1 << (uint8_t)i);
		}

		Hw_SCL_LOW();
		i--;
	}while(i>=0);

	Hw_SDA_HIGH();

	//Generates a clock pulse
	SCCB_DELAY();
	Hw_SCL_HIGH();
	SCCB_DELAY();
	Hw_SCL_LOW();

	if(data != NULL){
		*data = byte;
	}
}





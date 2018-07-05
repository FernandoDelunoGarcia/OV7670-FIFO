#include "Application.h"
#include "Fifo.h"
#include "OV7670.h"

#define QVGA_SIZE        (320*240)

int main(void)
{
	int state = 0;
	int flag;
	uint16_t tmp;
	uint16_t x, y;
	uint8_t pbuff[32];
	uint8_t cmd;

	//Hardware Init
	App_Init();

	for (;;) {

		switch(state)
		{
			case 0:
				flag = OV7670_Init();

				if(flag == 0){
					OV7670_Setup_QVGA_RGB565();
					state = 1;
				}
			break;

			case 1:
				flag = App_SerialRead((uint8_t *)&cmd);

				if(flag == 0){

					if((uint8_t)cmd == 'e'){
						state = 2;
					}
				}
			break;

			case 2:
				FIFO_StartCapture();

				for(x = 0; x<(QVGA_SIZE/16); x++){
					for(y = 0; y<16; y++){
						tmp = FIFO_GetPixel16();
						pbuff[2*y] = (uint8_t)tmp;
						pbuff[2*y + 1] = (uint8_t)(tmp >> 8);
					}

				   App_SerialWrite(pbuff, 32);
				}
				state = 1;
			break;
		}
    }

    return 0;
}

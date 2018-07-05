
#ifndef INCLUDES_FIFO_H_
#define INCLUDES_FIFO_H_

#include <stdint.h>

void FIFO_Init(void);
void FIFO_StartCapture(void);
uint16_t FIFO_GetPixel16(void);


#endif /* INCLUDES_FIFO_H_ */

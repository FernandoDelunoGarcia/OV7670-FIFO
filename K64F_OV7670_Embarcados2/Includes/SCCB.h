#ifndef INCLUDES_SCCB_H_
#define INCLUDES_SCCB_H_

#include "HardwareProxy.h"

void SCCB_Init(void);
void SCCB_ReadByte(uint8_t * data);
void SCCB_WriteByte(uint8_t data);
void SCCB_Start(void);
void SCCB_Stop(void);

#define SCCB_DELAY() Hw_Delay((1))
#define SCCB_ADDR_WR (0x21 << 1)

#endif

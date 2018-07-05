/*
 * HardwareProxy.c
 *
 *  Created on: Sep 29, 2017
 *      Author: Fernando
 */
#include "HardwareProxy.h"
#include "myDevice.h"
#include <stdlib.h>

static void Clock_Init();
static void Timer_Init();
static void IO_Init();
static void Serial_Init();

/* timeTicks */
static volatile uint32_t msTicks;


int Hw_Init(void)
{
	msTicks = 0;

	Clock_Init();
	IO_Init();
	Serial_Init();
	Timer_Init();

	return 0;
}

int Hw_SendByte(uint8_t data)
{
	while (!(UART0->S1 & (UART_S1_TDRE_MASK)));

	UART0->D = data;

	return 0;
}

int Hw_GetByte(uint8_t * data)
{
	if ((UART0->S1 & (UART_S1_RDRF_MASK)))
	{
		*data = UART0->D;

		return 0;
	}

	return -1;
}

void Hw_Delay (uint32_t dlyTicks) {

  uint32_t curTicks;

  curTicks = msTicks;

  while ((msTicks - curTicks) < dlyTicks);
}

void SysTick_Handler(void) {
  msTicks++;
}

static void Timer_Init(){
	SysTick_Config(SystemCoreClock/1000);
}

static void IO_Init(){

	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTC_MASK;


	PORTD_BASE_PTR->PCR[0] = PORT_PCR_MUX(1) | 3;
	PORTD_BASE_PTR->PCR[2] = PORT_PCR_MUX(1) | 3;

	//VSYNC
	PORTD_BASE_PTR->PCR[VSYNC_PIN] = PORT_PCR_MUX(1) |3;
	GPIOD_PDDR &= ~(1 << VSYNC_PIN);

	//WEN
	PORTD_BASE_PTR->PCR[WEN_PIN] = PORT_PCR_MUX(1) | 3;
	GPIOD_PDDR |= (1 << WEN_PIN);

	//WRST
	PORTA_BASE_PTR->PCR[WRST_PIN] = PORT_PCR_MUX(1) | 3;
	GPIOA_PDDR |= (1 << WRST_PIN);

	//RRST
	PORTA_BASE_PTR->PCR[RRST_PIN] = PORT_PCR_MUX(1) | 3;
	GPIOA_PDDR |= (1 << RRST_PIN);

	//OE
	PORTE_BASE_PTR->PCR[OE_PIN] = PORT_PCR_MUX(1) | 3;
	GPIOE_PDDR |= (1 << OE_PIN);

	//RCLK
	PORTE_BASE_PTR->PCR[RCLK_PIN] = PORT_PCR_MUX(1) | 3;
	GPIOE_PDDR |= (1 << RCLK_PIN);

	PORTB_BASE_PTR->PCR[22] = PORT_PCR_MUX(1);
	GPIOB_PDDR |= (1 << 22);
	GPIOB_PSOR = (1 << 22);

	//DATA line
	PORTC_BASE_PTR->PCR[0] = PORT_PCR_MUX(1) | 3;
	PORTC_BASE_PTR->PCR[1] = PORT_PCR_MUX(1) | 3;
	PORTC_BASE_PTR->PCR[2] = PORT_PCR_MUX(1) | 3;
	PORTC_BASE_PTR->PCR[3] = PORT_PCR_MUX(1) | 3;
	PORTC_BASE_PTR->PCR[4] = PORT_PCR_MUX(1) | 3;
	PORTC_BASE_PTR->PCR[5] = PORT_PCR_MUX(1) | 3;
	GPIOC_PDDR &= ~(PC_MASK);

	//DATA line
	PORTB_BASE_PTR->PCR[18] = PORT_PCR_MUX(1) | 3;
	PORTB_BASE_PTR->PCR[19] = PORT_PCR_MUX(1) | 3;
	GPIOB_PDDR &= ~(PB_MASK);
}

static void Serial_Init()
{
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_BASE_PTR->PCR[16] = PORT_PCR_MUX(3); /*RX*/
	PORTB_BASE_PTR->PCR[17] = PORT_PCR_MUX(3); /*TX*/

	SIM_BASE_PTR->SCGC4 |= SIM_SCGC4_UART0_MASK;
	UART0->C4 = 3; /*OSR - 1 / OSR = 4*/
	UART0->BDH = 0; /*parte alta de SBR = 0 / SBS = 0 indica 1 stop-bit*/
	UART0->BDL = 65; /*parte baixa de SBR = 65*/
	UART0->C1 &= ~(UART_C1_M_MASK | UART_C1_PE_MASK); /*8 bits, sem paridade*/
	UART0->C2 = (UART_C2_RE_MASK | UART_C2_TE_MASK); /*habilita transmissor e receptor*/
}

static void Clock_Init()
{
	SIM->CLKDIV1 = 0x01240000U;

    uint8_t reg = OSC->CR;
    reg &= ~(OSC_CR_SC2P_MASK | OSC_CR_SC4P_MASK | OSC_CR_SC8P_MASK | OSC_CR_SC16P_MASK);
    reg |= 0; //cap_load
    OSC->CR = reg;

    reg = OSC->CR;
    reg &= ~(OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK);
    reg |= 0x80; //enable mode
    OSC->CR = reg;

    MCG->C2 = ((MCG->C2 & ~(MCG_C2_EREFS0_MASK | MCG_C2_HGO0_MASK | MCG_C2_RANGE0_MASK)) | MCG_C2_RANGE(2) | (uint8_t)0);


    MCG->C7 = (MCG->C7 & ~MCG_C7_OSCSEL_MASK) | MCG_C7_OSCSEL(0);

	if (MCG->C2 & MCG_C2_EREFS_MASK)
	{
		while (!(MCG->S & MCG_S_OSCINIT0_MASK))
		{
		}
	}

	/*
       This function is designed to change MCG to PBE mode from PEE/BLPE/FBE,
       but with this workflow, the source mode could be all modes except PEI/PBI.
     */
    MCG->C2 &= ~MCG_C2_LP_MASK; /* Disable lowpower. */

    /* Change to use external clock first. */
    MCG->C1 = ((MCG->C1 & ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK)) | MCG_C1_CLKS(2));

    /* Wait for CLKST clock status bits to show clock source is ext ref clk */
    while ((MCG->S & (MCG_S_IREFST_MASK | MCG_S_CLKST_MASK)) !=
           (MCG_S_IREFST(0) | MCG_S_CLKST(2)))
    {
    }

    /* Disable PLL first, then configure PLL. */
    MCG->C6 &= ~MCG_C6_PLLS_MASK;
    while (MCG->S & MCG_S_PLLST_MASK)
    {
    }

    /* Configure the PLL. */
    uint8_t mcg_c5 = 0U;

    mcg_c5 |= MCG_C5_PRDIV0(0x13);
    MCG->C5 = mcg_c5; /* Disable the PLL first. */

    MCG->C6 = (MCG->C6 & ~MCG_C6_VDIV0_MASK) | MCG_C6_VDIV0(0x18);

    /* Set enable mode. */
    MCG->C5 |= ((uint32_t)64 | (uint32_t)0);

    /* Wait for PLL lock. */
    while (!(MCG->S & MCG_S_LOCK0_MASK))
    {
    }

    /* Change to PLL mode. */
    MCG->C6 |= MCG_C6_PLLS_MASK;
    while (!(MCG->S & MCG_S_PLLST_MASK))
    {
    }


	/* Change to use PLL output clock. */
    MCG->C1 = (MCG->C1 & ~MCG_C1_CLKS_MASK) | MCG_C1_CLKS(0);
    while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 3)
    {
    }


	/* Update FCRDIV. */
    MCG->SC = (MCG->SC & ~(MCG_SC_FCRDIV_MASK | MCG_SC_ATMF_MASK | MCG_SC_LOCS0_MASK)) | MCG_SC_FCRDIV(0);
	/* Set internal reference clock selection. */
    MCG->C2 = (MCG->C2 & ~MCG_C2_IRCS_MASK) | (MCG_C2_IRCS(0));
    MCG->C1 = (MCG->C1 & ~(MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK)) | (uint8_t)2;


	while (((MCG->S & MCG_S_IRCST_MASK) >> MCG_S_IRCST_SHIFT) != 0 /*ircs*/)
	{
	}


	SIM->CLKDIV1 = 0x1140000;
	SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_PLLFLLSEL_MASK) | SIM_SOPT2_PLLFLLSEL(1));
	SIM->SOPT1 = ((SIM->SOPT1 & ~SIM_SOPT1_OSC32KSEL_MASK) | SIM_SOPT1_OSC32KSEL(2));

	SystemCoreClockUpdate();
}

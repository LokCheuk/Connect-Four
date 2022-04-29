#include "stm32f10x.h"
#include "IERG3810_LED.h"

void IERG3810_LED_Init(void);

void IERG3810_LED_Init(void)
{
	// LED DS0
	RCC ->APB2ENR |= 1 << 3;
	GPIOB->CRL &= 0xFF0FFFFF; 
	GPIOB->CRL |= 0x00300000;
	GPIOB->BSRR = 1 << 5;
	
	// LED DS1
	RCC ->APB2ENR |= 1 << 6;
	GPIOE->CRL &= 0xFF0FFFFF; 
	GPIOE->CRL |= 0x00300000;
	GPIOE->BSRR = 1 << 5;
}


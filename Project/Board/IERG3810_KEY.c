#include "stm32f10x.h"
#include "IERG3810_KEY.h"

void IERG3810_KEY_Init(void);

void IERG3810_KEY_Init(void)
{

	//KEY1
	RCC ->APB2ENR |= 1 << 6;
	GPIOE->CRL &= 0xFFFF0FFF; 
	GPIOE->CRL |= 0x00008000; 
	GPIOE->BSRR |= 1 << 3;
	
	//KEY2
	RCC->APB2ENR |= 1 << 6;
	GPIOE->CRL &= 0xFFFFF0FF; 
	GPIOE->CRL |= 0x00000800; 
	GPIOE->BSRR |= 1 << 2;	
	
	//KEY_UP
	RCC ->APB2ENR |= 1 << 2;
	GPIOA->CRL &= 0xFFFFFFF0; 
	GPIOA->CRL |= 0x00000008; 
	GPIOA->BRR |= 1;
	
}


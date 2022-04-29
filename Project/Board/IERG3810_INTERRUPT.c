#include "stm32f10x.h"
#include "IERG3810_Delay.h"
#include "IERG3810_INTERRUPT.h"
#include "IERG3810_GameInit.h"


#define DS0_ON GPIOB ->BRR = 1 << 5
#define DS0_OFF GPIOB ->BSRR = 1 << 5
#define DS1_ON GPIOE ->BRR |= 1 << 5
#define DS1_OFF GPIOE ->BSRR |= 1 << 5
#define DS0_toggle GPIOB ->ODR ^= 1 << 5
#define DS1_toggle GPIOE ->ODR ^= 1 << 5

void IERG3810_NVIC_SetPriorityGroup(u8 prigroup)
{
	u32 temp, temp1;
	temp1 = prigroup & 0x00000007;
	temp1 <<= 8;
	temp = SCB->AIRCR;
	temp &= 0x0000F8FF;
	temp |= 0x05FA0000;
	temp |= temp1;
	SCB->AIRCR=temp;
}

void IERG3810_key2_ExtiInit(void)
{
	RCC->APB2ENR |= 1 <<6;
	GPIOE->CRL &= 0XFFFFF0FF;
	GPIOE->CRL |= 0X00000800;
	GPIOE->ODR |= 1 << 2;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFF0FF;
	AFIO->EXTICR[0] |= 0x00000400;
	EXTI->IMR |= 1<<2;
	EXTI->FTSR |= 1<<2;
	
	NVIC->IP[8] = 0x6E;
	NVIC->ISER[0] |= (1<<8);
}

void IERG3810_PS2key_ExtiInit(void)
{
	
	RCC->APB2ENR |= 1 << 4; 					// enable PC11
	GPIOC ->CRH &= 0xFFFF00FF;				// modify PC11
	GPIOC ->CRH |= 0x00008800;
	GPIOC ->BSRR = 1 << 11;				
	GPIOC ->BSRR = 1 << 10;	
	
	RCC->APB2ENR |= 0x01;							// enable AFIO clock
	AFIO ->EXTICR[2] &= 0xFFFF0FFF;
	AFIO ->EXTICR[2] |= 0x00002000;
	EXTI ->IMR |= 1 << 11; 						// edge trigger
	EXTI ->FTSR |= 1 << 11;						// falling edge
	
	NVIC ->IP[40] = 0x65; 						// set priority 
	NVIC ->ISER[1] |= (1 << 8);				// set NVIC 'SET ENABLE REGISTER'
}


void IERG3810_keyUP_ExtiInit(void)
{
	RCC->APB2ENR |= 1 <<2;
	GPIOA->CRL &= 0XFFFFFFF0;
	GPIOA->CRL |= 0X00000008;
	GPIOA->ODR |= 1 << 4;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFFFF0;
	AFIO->EXTICR[0] |= 0x00000000;
	EXTI->IMR |= 1<<0;
	EXTI->FTSR |= 1<<0;
	//EXTI->RTSR |= 1<<2;
	
	NVIC->IP[6] = 0x45;
	NVIC->ISER[0] |= (1<<6);
}

void EXTI2_IRQHandler(void)
{
	u8 i;
	for(i=0; i<10; i++)
	{
		DS0_ON;
		Delay(1000000);
		DS0_OFF;
		Delay(1000000);
	}
	EXTI->PR = 1<<2;
}

void EXTI0_IRQHandler(void)
{
	u8 i;
	for(i = 0; i < 10; i++)
	{
		DS1_ON;
		Delay(1000000);
		DS1_OFF;
		Delay(1000000);
	}
	EXTI ->PR = 1 << 0; //clear exception pending bit
}



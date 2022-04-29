#include "stm32f10x.h"
#include "IERG3810_TIMER.h"

void IERG3810_SYSTICK_Init10ms(void)
{
	SysTick->CTRL = 0;							// clear									
	SysTick->LOAD = 0x00015F8F;			// 72000000 / (8x100) - 1 = 89999 (0x00015F8F)
	SysTick->CTRL |= 0x00000003;		// set internal clock, use interrupt, start count
}

void IERG3810_TIM3_Init(u16 arr, u16 psc)
{
	RCC->APB1ENR |= 1 << 1;				
	TIM3->ARR = arr;						
	TIM3->PSC = psc;						
	TIM3->DIER |= 1 << 0;				
	TIM3->CR1 |= 0x01;					
	NVIC->IP[29] = 0x45;				
	NVIC->ISER[0] |= (1 << 29);	
}


void IERG3810_TIM3_PwmInit(u16 arr, u16 psc)
{
	RCC->APB2ENR |= 1 << 3;				// enable port B
	GPIOB ->CRL &= 0xFF0FFFFF;		// set analog mode, input mode (reset state)
	GPIOB ->CRL |= 0x00B00000; 		// set push-pull, max speed 50MHz
	
	RCC->APB2ENR |= 1 << 0; 			// enable the alternate function IO clock
	AFIO->MAPR &= 0xFFFFF3FF;			// set no remap (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)
	AFIO->MAPR |= 1 << 11;				// TIM3 remapping
	
	RCC->APB1ENR |= 1 << 1;				// enable the TIM3 clock
	TIM3->ARR = arr;							// set the auto-reload register
	TIM3->PSC = psc;							// set the counter clock frequency
	TIM3->CCMR1 |= 7 << 12;				// Output compare 1 clear enable, 
																// Capture/Compare 2 selection, 
												        // Output compare 2 fast enable
	
	TIM3->CCMR1 |= 1 << 11;				// Output compare 2 preload enable
	TIM3->CCER |= 1 << 4;					// Capture/Compare 2 output enable
	TIM3->CR1 = 0x0080;						// tDTS = tCK_INT
																// TIMx_ARR register is buffered, 
																// Edge-aligned mode. The counter counts up or down depending on the direction bit(DIR).
																// Counter used as upcounter
																// Counter is not stopped at update event
																// Counter is not stopped at update event
																// Update request source
																// Update disable
																	
	TIM3->CR1 |= 0x01;						// Counter disabled
}


void TIM3_IRQHandler(void)
{
	
	if(TIM3->SR & 1 << 0)
	{ 				
		GPIOB->ODR ^= 1 << 5;		
	}
	TIM3->SR &= ~(1 << 0);
}

void IERG3810_TIM4_Init(u16 arr, u16 psc)
{
	RCC->APB1ENR |= 1 << 2;				
	TIM4->ARR = arr;						
	TIM4->PSC = psc;						
	TIM4->DIER |= 1 << 0;				
	TIM4->CR1 |= 0x01;					
	NVIC->IP[30] = 0x45;				
	NVIC->ISER[0] |= (1 << 30);	
}

void TIM4_IRQHandler(void)
{
	if(TIM4->SR & 1 << 0)
	{ 				
		GPIOE->ODR ^= 1 << 5;				
	}
	TIM4->SR &= ~(1 << 0);
}

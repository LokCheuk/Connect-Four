#include "stm32f10x.h"
#include "IERG3810_USART.h"
void IERG3810_USART2_init(u32 pclk1,u32 bound)
{
	//USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float)(pclk1*1000000)/(bound*16);
	mantissa=temp;
	fraction=(temp-mantissa)*16;
	mantissa<<=4;
	mantissa+=fraction;
	RCC->APB2ENR|=1<<2;				 // (RM0008 page 112)
	RCC->APB1ENR|=1<<17;			 // (RM0008 page 115)
	GPIOA->CRL&=0xFFFF00FF;		 // set PA2, PA3 Alternate Function 
	GPIOA->CRL|=0X00008B00;		 // set PA3, PA3 Alternate Function 
	RCC->APB1RSTR|=1<<17;			 // (RM0008 page 109)
	RCC->APB1RSTR&=~(1<<17);	 // (( ~mean inverted)
	USART2->BRR=mantissa;			 // (RM0008 page 820)
	USART2->CR1|=0x2008;			 // (RM0008 page 821)
	
}

void IERG3810_USART1_init(u32 pclk2, u32 bound){
	//USART1
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float)(pclk2*1000000)/(bound*16);
	mantissa=temp;
	fraction=(temp-mantissa)*16;
	mantissa<<= 4;
	mantissa+=fraction;
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<14;
	GPIOA->CRH&=0xFFFFF00F;
	GPIOA->CRH|=0x000008B0;
	RCC->APB2RSTR|=1<<14;
	RCC->APB2RSTR&=~(1<<14);
	USART1->BRR=mantissa;
	USART1->CR1|= 0x2008;
}


void USART_print(u8 USARTport, char *st)
{
	u8 i=0;
	while (st[i] != 0x00) 
	{
		if (USARTport == 1) {
			USART1->DR = st[i]; 
			while((USART1->SR &0x00000080) >>7 != 1); 
				
		}
		
		if (USARTport == 2) {
			USART2->DR = st[i];
			while((USART2->SR &0x00000080) >>7 != 1); 
		}
		
		if (i == 255) break;
		i++;
	}	
}


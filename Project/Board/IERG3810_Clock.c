#include "stm32f10x.h"
#include "IERG3810_Clock.h"

void IERG3810_clock_tree_init(void)
{
	u8 PLL=7;
	unsigned char temp=0; 	 // used 8-bits only
	RCC->CFGR &= 0xF8FF0000; // clear bits [15:0], (RCC->CFGR, RM0008 p.101)
													 // RCC->CR(RM0008 p.99)
	RCC -> CR &= 0xFEF6FFFF; // PLLON = 0 [24], CSSON=0 [19], HSEON=0 [16]
	RCC->CR|=0x00010000;		 // HSEON=1 [16]
	while(!(RCC->CR>>17));	 // Check HSERDY
	RCC->CFGR=0X00000400;		 // PPRE1=100 [10:8], set APB1 (HCLK divided by 2)
	RCC->CFGR|=PLL<<18;			 // PLLMUL=111 [21:18], set PLL input clock x 9
	RCC->CFGR|=1<<16;				 // PLLSRC = 1 [16]
													 // FLASH->ACR (RM0008 p.60)
	FLASH->ACR|=0x32;				 //	72MHz should set FLASH with 2 wait states,
	RCC->CR|=0x01000000;		 // PLLON = 1 [24]
	while(!(RCC->CR>>25));	 // check PLLRDY [25]
	RCC->CFGR|=0x00000002;	 // SW=10 [1:0]
	while(temp!=0x02)				 // check SWS[1:0] (RM0008 p.101)
	{
		temp=RCC->CFGR>>2;		 // - check SWS[1:0] status
		temp&=0x03;						 // keep last two bits
	}
}


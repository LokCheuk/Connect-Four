#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "stm32f10x.h"
#include "IERG3810_Clock.h"
#include "IERG3810_USART.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_LED.h"
#include "IERG3810_KEY.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_TIMER.h"
#include "IERG3810_Delay.h"
#include "IERG3810_INTERRUPT.h"

#include "IERG3810_InitialPage.h"
#include "IERG3810_GameBoard.h"
#include "IERG3810_GameInit.h"
#include "IERG3810_GameRules.h"
#include "global.h"

u8 task3HeartBeat = 0;

int main(void)
{
		u32 sheep=0, timeout = 10000, ps2count = 0;
		u8 ps2key = 0;
		IERG3810_clock_tree_init();
		IERG3810_USART2_init(36,9600);
		IERG3810_USART1_init(72,9600);

		IERG3810_LED_Init();
		IERG3810_SYSTICK_Init10ms();
		IERG3810_NVIC_SetPriorityGroup(5);
		IERG3810_PS2key_ExtiInit();

		IERG3810_TFTLCD_Init();
		IERG3810_TFTLCD_FillRectangle(0x0000,0,1000,0,1000);
		IERG3810_initialpage();

		while (task3HeartBeat<201){}	// 2 seconds

		IERG3810_TFTLCD_FillRectangle(0x0000,0,1000,0,1000);

		IERG3810_Game_restart();

		while(1)
		{
			sheep++;
			
			if(ps2count >= 11)
			{
				EXTI ->PR = 1 << 11;
			}
			timeout --;
			
			if(timeout == 0)
			{
				timeout = 20000;
				ps2key = 0;
				ps2count = 0;
			}

		}
}

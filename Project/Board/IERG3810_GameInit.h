#ifndef __IERG3810_GameInit_H
#define __IERG3810_GameInit_H
#include "stm32f10x.h"

void IERG3810_Game_Init(void);

void IERG3810_chessinsert(int player, int column);

int IERG3810_checkdraw();

int IERG3810_checkwin(int player);

void IERG3810_turnchange();

void IERG3810_move(u8 move);

void IERG3810_result_show(u8 result);

void IERG3810_Game_restart(void);

void IERG3810_shiftbit(void);

void IERG3810_detectbit(void);

void EXTI15_10_IRQHandler(void);

#endif

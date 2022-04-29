#include "stm32f10x.h"
#include "time.h"
#include "IERG3810_USART.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_INTERRUPT.h"
#include "IERG3810_Delay.h"
#include "IERG3810_TIMER.h"
#include "global.h"
#include "IERG3810_GameRules.h"
#include "IERG3810_GameBoard.h"

int chessboard[6][7];		// chessboard array
int numofchess[7]; 			// number of chess in each column
int totalnumofchess;		// total number of chess in the chessboard
int turn;								// turn number
int column;							// column number

u32 sheep=0;
u32 timeout=10000;
u8 ps2key = 0;
u32 ps2count =0;
u8 ps2dataReady = 0;
u8 ps2Buffer[10];

u8 task1HeartBeat = 0;
u8 task2HeartBeat = 0;
u32 task4HeartBeat = 0;

#define DS0_ON GPIOB ->BRR = 1 << 5
#define DS0_OFF GPIOB ->BSRR = 1 << 5
#define DS1_ON GPIOE ->BRR |= 1 << 5
#define DS1_OFF GPIOE ->BSRR |= 1 << 5
#define DS0_toggle GPIOB ->ODR ^= 1 << 5
#define DS1_toggle GPIOE ->ODR ^= 1 << 5

void IERG3810_Game_Init(void)					// Game Initialization
{
		int i, j;
		totalnumofchess = 0;						// set total number of chess in the chessboard to be 0
		turn = 1;												// set player 1 move first
		column = 0;											// set column to be 0 such that the arrow can appear ar column 0
		
		for (i=0;i<6;i++)
		{
			for (j=0;j<7;j++)
			{
				chessboard[i][j] = 0;				// clear the chessboard (set to 0)
			}
		}
		
		for (i=0;i<=6;i++)
		{	
			numofchess[i] = 0;						// initialize the number of chess in each column to be 0
		}
		
} 

void IERG3810_chessinsert(int player, int column)	// insert chess
{
		u8 x = 15 * (column + column + 2) - 7;
		u8 y = 20 + (numofchess[column] + 1) * 15 + numofchess[column] * 15 - 8;
	 
		if(player == 1) // insert player 1 chess
		{ 
				chessboard[numofchess[column]][column] = 1;							// add one piece in the column
				IERG3810_TFTLCD_ShowCharacter(x, y, 0, 0x7E0, 0x0000);	// show player 1 piece
				numofchess[column]++; 																	// num of chess in the column + 1			
				totalnumofchess++;																			// total number of chess + 1

		}
		
		if(player == 2) // insert player 2 chess
		{
				chessboard[numofchess[column]][column] = 2;							// add one piece in the column
				IERG3810_TFTLCD_ShowCharacter(x, y, 0, 0xF800, 0x0000);	// show player 2 piece
				numofchess[column]++; 																	// num of chess in the column + 1
				totalnumofchess++;																			// total number of chess + 1

		}				
}


int IERG3810_checkdraw()								// check draw condition (total number of chess = 42)
{
		if(totalnumofchess == 42) 
		{
			return 0;
		}
		
		return 1;
			
}

int IERG3810_checkwin(int player)				// check win conditions (horizontal, vertical, oblique(ascending, decending))
{
		int i,j;
	
		// horizontal
		for (j=0;j<=3;j++) 
		{
			for (i=0;i<=5;i++)
			{
				if (chessboard[i][j]==player && chessboard[i][j+1]==player && chessboard[i][j+2]==player && chessboard[i][j+3]==player)
				{
					return 1;
				}
			}
		}
		
		// vertical
		for (i=0;i<=2;i++) 
		{
			for (j=0;j<=6;j++)
			{
				if (chessboard[i][j]==player && chessboard[i+1][j]==player && chessboard[i+2][j]==player && chessboard[i+3][j]==player)
				{
					return 1;
				}
			}
		}
		
		// oblique (ascending)
		for (i=3;i<=5;i++)
		{
			for (j=0;j<=3;j++)
			{
				if (chessboard[i][j]==player && chessboard[i-1][j+1]==player && chessboard[i-2][j+2]==player && chessboard[i-3][j+3]==player)
				{
					return 1;
				}
			}
		}
		
		// oblique (decending)
		for (i=3;i<=5;i++)
		{
			for (j=3;j<=6;j++)
			{
				if (chessboard[i][j]==player && chessboard[i-1][j-1]==player && chessboard[i-2][j-2]==player && chessboard[i-3][j-3]==player)
				{
					return 1;
				}
			}
		}
		
		return 0;
}

void IERG3810_turnchange()							// turn change function (LED on)
{
		if (turn == 1)
		{			
				DS0_ON;
				DS1_OFF;
				turn = 2;
		}
		
		else
		{							
				DS0_OFF;
				DS1_ON;
				turn = 1;	
		}
}


void IERG3810_move(u8 move)							// move the arrow 
{
		u8 x = 15 * (column + column + 2) - 7 ;
	
		if (move == 1)
		{
				column = 0 ; // 23
				IERG3810_TFTLCD_FillRectangle(0x0000,23,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,53,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,83,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,113,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,143,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,173,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,203,16, 250, 16);
				IERG3810_TFTLCD_ShowCharacter(x ,250,1 ,0xFFFF,0x0000);
		}
		
		if (move == 2)
		{
				column = 1; // 53
				IERG3810_TFTLCD_FillRectangle(0x0000,23,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,53,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,83,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,113,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,143,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,173,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,203,16, 250, 16);
				IERG3810_TFTLCD_ShowCharacter(x ,250,1 ,0xFFFF,0x0000);
		}
		
		if (move == 3)
		{
				column = 2; // 83
				IERG3810_TFTLCD_FillRectangle(0x0000,23,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,53,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,83,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,113,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,143,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,173,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,203,16, 250, 16);
				IERG3810_TFTLCD_ShowCharacter(x ,250,1 ,0xFFFF,0x0000);
		}		
		
		if (move == 4)
		{
				column = 3; // 113
				IERG3810_TFTLCD_FillRectangle(0x0000,23,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,53,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,83,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,113,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,143,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,173,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,203,16, 250, 16);		
				IERG3810_TFTLCD_ShowCharacter(x ,250,1 ,0xFFFF,0x0000);
		}
		
		if (move == 5)
		{
				column = 4; // 143
				IERG3810_TFTLCD_FillRectangle(0x0000,23,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,53,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,83,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,113,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,143,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,173,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,203,16, 250, 16);		
				IERG3810_TFTLCD_ShowCharacter(x ,250,1 ,0xFFFF,0x0000);
		}
		
		if (move == 6)
		{
				column = 5; // 173
				IERG3810_TFTLCD_FillRectangle(0x0000,23,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,53,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,83,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,113,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,143,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,173,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,203,16, 250, 16);
				IERG3810_TFTLCD_ShowCharacter(x ,250,1 ,0xFFFF,0x0000);
		}
		
		if (move == 7)
		{
				column = 6; // 203
				IERG3810_TFTLCD_FillRectangle(0x0000,23,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,53,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,83,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,113,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,143,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,173,16, 250, 16);
				IERG3810_TFTLCD_FillRectangle(0x0000,203,16, 250, 16);
				IERG3810_TFTLCD_ShowCharacter(x ,250,1 ,0xFFFF,0x0000);
		}		
}

void IERG3810_result_show(u8 result)		// display the result (player 1/2 wins / draw)
{
		IERG3810_TFTLCD_FillRectangle(0x0000,0,1000,0,1000);
	
		if (result == 2) // player 1 wins
		{	
			char player1[15] = {"Player 1 wins\n"};
			USART_print(1, player1);

			//P
			IERG3810_TFTLCD_FillRectangle(0xFFFF,65,5,190,60);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,70,5,220,5);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,70,5,245,5);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,75,5,220,30);	
			//L
			IERG3810_TFTLCD_FillRectangle(0xFFFF,85,15,190,5);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,85,5,195,55);	
			//A
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,5,190,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,115,5,190,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,110,5,210,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,110,5,230,20);	
			//Y
			IERG3810_TFTLCD_FillRectangle(0xFFFF,125,5,220,30);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,130,5,190,40);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,135,5,220,30);
			//E
			IERG3810_TFTLCD_FillRectangle(0xFFFF,145,5,190,60);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,150,10,190,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,150,10,215,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,150,10,240,10);
			//R
			IERG3810_TFTLCD_FillRectangle(0xFFFF,165,5,190,60);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,170,5,240,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,170,5,210,20);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,175,5,220,30);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,173,5,200,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,178,5,190,10);
			//1
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,5,160,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,110,5,170,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,25,120,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,115,5,130,50);
			
			IERG3810_TFTLCD_FillRectangle(0xFFFF,85,5,60,50);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,95,5,60,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,5,60,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,90,5,50,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,100,5,50,10);	
			//I
			IERG3810_TFTLCD_FillRectangle(0xFFFF,115,5,50,60);	
			//N
			IERG3810_TFTLCD_FillRectangle(0xFFFF,125,5,50,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,140,5,50,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,130,10,100,10);	
			
			task2HeartBeat = 0;
			while (task2HeartBeat<201) {} // 2 seconds
			
		}
		
		else if (result == 1) // player 2 wins
		{
			char player2[15] = {"Player 2 wins\n"};
			USART_print(1, player2);

			//P
			IERG3810_TFTLCD_FillRectangle(0xFFFF,65,5,190,60);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,70,5,220,5);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,70,5,245,5);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,75,5,220,30);	
			//L
			IERG3810_TFTLCD_FillRectangle(0xFFFF,85,15,190,5);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,85,5,195,55);	
			//A
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,5,190,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,115,5,190,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,110,5,210,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,110,5,230,20);	
			//Y
			IERG3810_TFTLCD_FillRectangle(0xFFFF,125,5,220,30);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,130,5,190,40);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,135,5,220,30);
			//E
			IERG3810_TFTLCD_FillRectangle(0xFFFF,145,5,190,60);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,150,10,190,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,150,10,215,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,150,10,240,10);
			//R
			IERG3810_TFTLCD_FillRectangle(0xFFFF,165,5,190,60);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,170,5,240,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,170,5,210,20);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,175,5,220,30);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,173,5,200,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,178,5,190,10);
			//2
			IERG3810_TFTLCD_FillRectangle(0xFFFF,110,15,130,5);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,5,130,20);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,15,150,5);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,120,5,150,20);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,20,170,5);

			//W
			IERG3810_TFTLCD_FillRectangle(0xFFFF,85,5,60,50);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,95,5,60,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,105,5,60,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,90,5,50,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,100,5,50,10);	
			//I
			IERG3810_TFTLCD_FillRectangle(0xFFFF,115,5,50,60);	
			//N
			IERG3810_TFTLCD_FillRectangle(0xFFFF,125,5,50,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,140,5,50,50);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,130,10,100,10);	
			
			task2HeartBeat = 0;
			while (task2HeartBeat<201) {} // 2 seconds

		}
		
		else if (result == 3) // draw
		{
			char draw[6] = {"Draw\n"};
			USART_print(1, draw);

			//D
			IERG3810_TFTLCD_FillRectangle(0xFFFF,30,30,150,20);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,30,30,200,20);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,30,10,170,30);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,60,10,160,50);
			//R
			IERG3810_TFTLCD_FillRectangle(0xFFFF,80,10,150,30);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,90,10,170,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,100,10,160,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,110,10,150,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,80,40,180,40);
			IERG3810_TFTLCD_FillRectangle(0x0000,90,20,190,20);
			//A
			IERG3810_TFTLCD_FillRectangle(0xFFFF,130,10,150,60);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,160,10,150,60);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,140,20,170,10);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,140,20,200,20);
			//W
			IERG3810_TFTLCD_FillRectangle(0xFFFF,190,10,150,10);
		  IERG3810_TFTLCD_FillRectangle(0xFFFF,210,10,150,10);	
			IERG3810_TFTLCD_FillRectangle(0xFFFF,180,10,160,60);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,200,10,160,60);
			IERG3810_TFTLCD_FillRectangle(0xFFFF,220,10,160,60);
			
			task2HeartBeat = 0;
			while (task2HeartBeat<201){} // 2 seconds

		}
}


void IERG3810_Game_restart(void)				// Game start/restart
{
			int i;
			char message[15] = {"Game Started\n"};
			USART_print(1, message);					// send USART1 message
			
			IERG3810_TFTLCD_FillRectangle(0x0000,0,1000,0,1000);
						
			task4HeartBeat = 0;
	
			while (task4HeartBeat<501)				// count 5 seconds
			{				
				IERG3810_GameRules();
			}
			
			IERG3810_TFTLCD_FillRectangle(0x0000,0,1000,0,1000);
			
			for (i=5;i!=-1;--i)
			{
				IERG3810_TFTLCD_SevenSegment(0xFFFF,80,80,i);
				
				task1HeartBeat = 0;
				while (task1HeartBeat<101){}		// count 1 second
					
				IERG3810_TFTLCD_SevenSegment(0x0000,80,80,i);
			}
			
			IERG3810_Game_Init();
			IERG3810_GameBoard_Init();
			IERG3810_turnchange();
			IERG3810_move(1);
}

void IERG3810_shiftbit(void)						// shifting the bits
{
			u8 index = 0;
			for(index = 0; index < 9; index ++)
			{
				ps2Buffer[index] = ps2Buffer[index + 1]; 
			}
			ps2Buffer[9] = ps2key;
}

void IERG3810_detectbit(void)						// detect the PS2 key and actions
{
			if(ps2Buffer[8] != 0xF0)
			{
				if (ps2Buffer[9] == 0x69) 			// 1
				{
					IERG3810_move(1);
				}
				
				else if (ps2Buffer[9] == 0x72) 	// 2
				{
					IERG3810_move(2);
				}
				
				else if (ps2Buffer[9] == 0x7A) 	// 3
				{
					IERG3810_move(3);
				}
				
				else if (ps2Buffer[9] == 0x6B)	// 4
				{
					IERG3810_move(4);
				}
				
				else if (ps2Buffer[9] == 0x73) 	// 5
				{
					IERG3810_move(5);
				}
				
				else if (ps2Buffer[9] == 0x74) 	// 6
				{
					IERG3810_move(6);
				}
				
				else if (ps2Buffer[9] == 0x6C)	// 7
				{
					IERG3810_move(7);
				}	
				
				else if(ps2Buffer[9] == 0x5A) 	// Enter
				{
					if(numofchess[column]<6)
					{
						IERG3810_chessinsert(turn, column);	// insert piece

						if (IERG3810_checkwin(turn) == 1)		// check which player wins
						{
							task1HeartBeat = 0;
							while (task1HeartBeat<201){}			// count 2 seconds
							IERG3810_result_show(turn);				// show win result
							DS0_OFF;													// off all the LED
							DS1_OFF;
							IERG3810_Game_restart();					// restart the game automatically
						}
						
						else if(IERG3810_checkdraw() == 0)	// check whether draw or not
						{
							task1HeartBeat = 0;
							while (task1HeartBeat<201){}			// count 2 seconds
							IERG3810_result_show(3);					// show draw result
							DS0_OFF;													// off all LED
							DS1_OFF;
							IERG3810_Game_restart();					// restart the game automatically
						}
						
						else
						{	
							IERG3810_turnchange();						// if no one wins or draw, the game continues and change turn
						}
					}
				}

			}
}	

void EXTI15_10_IRQHandler(void)					// interrupt handler for PS2 keypad
{
		if(ps2count == 0)
		{
			if(((GPIOC->IDR>>10)&1) == 0) // skip start bit
			{
				ps2key = 0;
				timeout = 20000;
				ps2count ++;		
			}
		}

		else if(ps2count <= 8 && ps2count > 0) // 1st - 8th bit
		{ 
			ps2dataReady = ps2key;
			ps2dataReady >>= 1;  

			if( ((GPIOC->IDR>>10)&1) != 0) 
			{
				ps2dataReady |= 0x80;
			}
			ps2key = ps2dataReady;
			ps2count ++; 
		}
		
		else if(ps2count == 9) //skip 9th bit
		{
			ps2count ++;
		}

		else if (ps2count <= 10)
		{
			if(((GPIOC->IDR>>10)&1) != 0)
			{
				IERG3810_shiftbit();
				IERG3810_detectbit();
			}
			ps2count = 0;
		}

		EXTI ->PR = 1 << 11; //clear exception pending bit
}

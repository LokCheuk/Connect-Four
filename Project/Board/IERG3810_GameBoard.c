#include "stm32f10x.h"
#include "IERG3810_GameBoard.h"
#include "IERG3810_TFTLCD.h"

void IERG3810_GameBoard_Init(void)
{
	u32 i;
	u32 j;

	for (i=15;i<=225;i++)
	{
		// horizontal line
		IERG3810_TFTLCD_DrawDot(i,20,0xFFFF);		// print the white horizontal line
		IERG3810_TFTLCD_DrawDot(i,50,0xFFFF);		// print the white horizontal line
		IERG3810_TFTLCD_DrawDot(i,80,0xFFFF);		// print the white horizontal line
		IERG3810_TFTLCD_DrawDot(i,110,0xFFFF);	// print the white horizontal line
		IERG3810_TFTLCD_DrawDot(i,140,0xFFFF);	// print the white horizontal line
		IERG3810_TFTLCD_DrawDot(i,170,0xFFFF);	// print the white horizontal line
		IERG3810_TFTLCD_DrawDot(i,200,0xFFFF);	// print the white horizontal line
	}
	
	for (j=20;j<=200;j++)
	{
		// vertical line
		IERG3810_TFTLCD_DrawDot(15,j,0xFFFF);		// print the white vertical line
		IERG3810_TFTLCD_DrawDot(45,j,0xFFFF);		// print the white vertical line
		IERG3810_TFTLCD_DrawDot(75,j,0xFFFF);		// print the white vertical line
		IERG3810_TFTLCD_DrawDot(105,j,0xFFFF);	// print the white vertical line
		IERG3810_TFTLCD_DrawDot(135,j,0xFFFF);	// print the white vertical line
		IERG3810_TFTLCD_DrawDot(165,j,0xFFFF);	// print the white vertical line
		IERG3810_TFTLCD_DrawDot(195,j,0xFFFF);	// print the white vertical line
		IERG3810_TFTLCD_DrawDot(225,j,0xFFFF);	// print the white vertical line
	}
	
	IERG3810_TFTLCD_ShowChar(25, 210, 49, 0xFFFF, 0x0000); 			// print column 1
	IERG3810_TFTLCD_ShowChar(55, 210, 50, 0xFFFF, 0x0000); 			// print column 2
	IERG3810_TFTLCD_ShowChar(85, 210, 51, 0xFFFF, 0x0000); 			// print column 3
	IERG3810_TFTLCD_ShowChar(115, 210, 52, 0xFFFF, 0x0000); 		// print column 4
	IERG3810_TFTLCD_ShowChar(145, 210, 53, 0xFFFF, 0x0000); 		// print column 5
	IERG3810_TFTLCD_ShowChar(175, 210, 54, 0xFFFF, 0x0000); 		// print column 6
	IERG3810_TFTLCD_ShowChar(205, 210, 55, 0xFFFF, 0x0000); 		// print column 7
}

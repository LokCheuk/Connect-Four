#include "stm32f10x.h"
#include "IERG3810_Delay.h"

void Delay(u32 count)
{
	u32 i;
	for (i=0; i<count; i++);
}

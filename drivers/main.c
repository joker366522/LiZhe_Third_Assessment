#include "main.h"
//#include "MacroAndConst.h"

float limit = 2000;
int main(void)
{  
	CAN1_Mode_Init();
	TIM3_Int_Init();
	PidInit();
	remote_config();
    while(1){}
}

//CAN_ClearITPendingBit(CAN1, CAN_IT_FF0); //0满中断
//CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0); //0消息挂起中断
	



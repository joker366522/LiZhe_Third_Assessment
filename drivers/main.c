#include "main.h"
//#include "MacroAndConst.h"

float limit = 2000;
int main(void)
{  
    CAN1_Mode_Init();
	CAN2_Mode_Init();
	 
	PidInit();
	uart5_init();
	
	imu_data_decode_init(); 
	
	remote_config();
	StraightInit();
	TIM3_Int_Init();//定时器初始化之前，读取当前的角度值作为初始化角度
    while(1){}
}

//CAN_ClearITPendingBit(CAN1, CAN_IT_FF0); //0满中断
//CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0); //0消息挂起中断
	



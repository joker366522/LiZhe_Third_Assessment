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
	TIM3_Int_Init();//��ʱ����ʼ��֮ǰ����ȡ��ǰ�ĽǶ�ֵ��Ϊ��ʼ���Ƕ�
    while(1){}
}

//CAN_ClearITPendingBit(CAN1, CAN_IT_FF0); //0���ж�
//CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0); //0��Ϣ�����ж�
	



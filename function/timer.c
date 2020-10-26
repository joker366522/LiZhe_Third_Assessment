#include "main.h"

int Arr = 10 - 1;
int Psc = 8400-1;

short Acc[3];
short Gyo[3]; //Gyo[2]
short Mag[3];
float Eular[3]; //Eular[2]是我需要的实际旋转角度
float Quat[4];
unsigned char ID;

void TIM3_Int_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = Arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Psc;         
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //相当于预分频后的值与总线的频率相当 
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;               
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter;  高级定时器专有
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM3,ENABLE);//开启定时器
}

float a1,a2,a3,a4,b1,w1_ratio; 
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)//判断是否发生了更新(溢出)中断
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除溢出中断标志位
		
		get_raw_acc(Acc);
        get_raw_gyo(Gyo);
        get_raw_mag(Mag);
        get_eular(Eular);
        get_quat(Quat);
        get_id(&ID);
		change();
		
		w1_ratio = Circle();//StraightCalculation();
		a1 = PidCalculation1();
		a2 = PidCalculation2();
		a3 = PidCalculation3();
		a4 = PidCalculation4(); 
		b1 = PidCalculation_UP12();
		if(out == 1) 
		{
			CAN1_Send_Msg(0,0,0,0);
			CAN2_Send_Msg(0,0);
		}
		else if(out == 3)
		{
			CAN1_Send_Msg(a1,a2,a3,a4);
			CAN2_Send_Msg(b1,b1); 
		}
		else if(out == 2)
		{
			//CAN1_Send_Msg(a1,a2,a3,a4);
			CAN1_Send_Msg(a1,a2,a3,a4);
			CAN2_Send_Msg(b1,b1); 
		}
		
		//a1=a2=a3=a4=0;
    }
}



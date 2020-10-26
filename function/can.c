#include "main.h"

int data[8];
float DataFinal[2];

motor fpchassis = //实体的创建与初始化
{
	.Speed_wheel  = 0,
	.Mechanical_wheel = 0,
	.torque_current_wheel = 0,
	.temperature = 0,
	.Mechanical_Yaw_Angle 	= 0,.Speed_Yaw_Angle = 0,
	.Mechanical_Pitch_Angle = 0,.Speed_Pitch_Angle = 0,
	.Mechanical_Roll_Angle 	= 0,.Speed_Roll_Angle = 0,
	.flag 	= 0, //接收完成标志位
	
	.LF_current  = 0, //四轮的目标转速
	.RF_current  = 0,
	.LR_current  = 0,
	.RR_current  = 0,
};

/*数据包解算*/
void DataProcessing(CanRxMsg RxMessage)
{
	switch(RxMessage.StdId)
		{
			case 0x201: fpchassis.Mechanical_wheel[0]			= (float)((int16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]));
					    fpchassis.Speed_wheel[0] 			    = (float)((int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]));
						fpchassis.torque_current_wheel[0]       = (float)((int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]));
						fpchassis.temperature[0]				= (float)((int16_t)(RxMessage.Data[6]));break;
			case 0x202:	fpchassis.Mechanical_wheel[1]			= (float)((int16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]));
						fpchassis.Speed_wheel[1] 				= (float)((int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]));
						fpchassis.torque_current_wheel[1]       = (float)((int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]));
						fpchassis.temperature[1]				= (float)((int16_t)(RxMessage.Data[6]));break;
			case 0x203:	fpchassis.Mechanical_wheel[2]			= (float)((int16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]));
						fpchassis.Speed_wheel[2] 				= (float)((int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]));
						fpchassis.torque_current_wheel[2]       = (float)((int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]));
						fpchassis.temperature[2]				= (float)((int16_t)(RxMessage.Data[6]));break;
			case 0x204:	fpchassis.Mechanical_wheel[3]			= (float)((int16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]));
						fpchassis.Speed_wheel[3] 				= (float)((int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]));
						fpchassis.torque_current_wheel[3]       = (float)((int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]));
						fpchassis.temperature[3]				= (float)((int16_t)(RxMessage.Data[6]));break;
			
			
			case 0x205:	fpchassis.Mechanical_Yaw_Angle 		    = (float)((int16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]));
						fpchassis.Speed_Yaw_Angle 				= (float)((int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]));
						fpchassis.torque_current_wheel[4]       = (float)((int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]));
						fpchassis.temperature[4]				= (float)((int16_t)(RxMessage.Data[6]));break;
			case 0x206:	fpchassis.Mechanical_Pitch_Angle 		= (float)((int16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]));
						fpchassis.Speed_Pitch_Angle 			= (float)((int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]));
						fpchassis.torque_current_wheel[5]       = (float)((int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]));
						fpchassis.temperature[5]				= (float)((int16_t)(RxMessage.Data[6]));break;
			case 0x207:	fpchassis.Mechanical_Roll_Angle 		= (float)((int16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]));
						fpchassis.Speed_Roll_Angle 				= (float)((int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]));
						fpchassis.torque_current_wheel[6]       = (float)((int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]));
						fpchassis.temperature[6]				= (float)((int16_t)(RxMessage.Data[6]));break;
		}
		fpchassis.flag = 1;
}

void CAN1_Mode_Init(void)
{
	/*重命名*/
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef	CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*使能时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	/*GPIO的配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*引脚复用*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1);
	
	/*CAN单元设置*/
	//CAN_DeInit(CAN1);
	CAN_InitStructure.CAN_TTCM = DISABLE;//不使能时间触发模式
	CAN_InitStructure.CAN_ABOM = DISABLE;//不使能自动离线管理
	CAN_InitStructure.CAN_AWUM = DISABLE;//不使能自动唤醒模式
	CAN_InitStructure.CAN_NART = DISABLE;//不使能非自动重传模式
	CAN_InitStructure.CAN_RFLM = DISABLE;//不使能接受FIFO锁定模式
	CAN_InitStructure.CAN_TXFP = ENABLE;//使能发送FIFO优先级，优先由报文标识符决定
	//在主函数中初始化
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //重新同步跳跃宽度为1个时间单位
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //时间段1占用一个时间单位
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //时间段2占用7个时间单位
	CAN_InitStructure.CAN_Prescaler = 3; //分频系数
	CAN_Init(CAN1,&CAN_InitStructure); 
	
	/*配置过滤器*/
	CAN_FilterInitStructure.CAN_FilterNumber = 0; //指定过滤器为0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//标识符列表模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;//过滤器位宽为32
	CAN_FilterInitStructure.CAN_FilterIdHigh      = 0x0000; //要过滤的ID高位和低位
	CAN_FilterInitStructure.CAN_FilterIdLow       = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh  = 0x0000; //过滤器屏蔽标识
	CAN_FilterInitStructure.CAN_FilterMaskIdLow   = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//设定了指向过滤器的FIFO为0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//过滤器使能
	CAN_FilterInit(&CAN_FilterInitStructure); 
	
	/*配置中断*/
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIF00信息挂号允许，使能
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1; //主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/*写一个中断函数用来发送数据，清空主函数中的while(1)死循环*/
void CAN_Send_Msg(int a1,int a2,int a3,int a4)
{
	CanTxMsg txMessage;
	
	txMessage.RTR = CAN_RTR_Data; //数据帧而不是遥控帧
	txMessage.IDE = CAN_Id_Standard; //使用标准标识符
	txMessage.StdId = 0x200; //标准标识符，发送的标准标识符
	txMessage.DLC = 0x08; //消息的数据长度为8个字节
	
	txMessage.Data[0] = (int)(a1>>8); //因为负值
    txMessage.Data[1] = (int) a1;
    txMessage.Data[2] = (int)(a2>>8);
    txMessage.Data[3] = (int) a2;
    txMessage.Data[4] = (int)(a3>>8);
    txMessage.Data[5] = (int) a3;
    txMessage.Data[6] = (int)(a4>>8);
    txMessage.Data[7] = (int) a4;
	
	CAN_Transmit(CAN1,&txMessage);	
}

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		DataProcessing(RxMessage);	
	}
}




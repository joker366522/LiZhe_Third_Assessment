#include "main.h"

int data[8];
float DataFinal[2];

motor fpchassis = //ʵ��Ĵ������ʼ��
{
	.Speed_wheel  = 0,
	.Mechanical_wheel = 0,
	.torque_current_wheel = 0,
	.temperature = 0,
	.Mechanical_Yaw_Angle 	= 0,.Speed_Yaw_Angle = 0,
	.Mechanical_Pitch_Angle = 0,.Speed_Pitch_Angle = 0,
	.Mechanical_Roll_Angle 	= 0,.Speed_Roll_Angle = 0,
	.flag 	= 0, //������ɱ�־λ
	
	.LF_current  = 0, //���ֵ�Ŀ��ת��
	.RF_current  = 0,
	.LR_current  = 0,
	.RR_current  = 0,
};

/*���ݰ�����*/
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
	/*������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef	CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*ʹ��ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	/*GPIO������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*���Ÿ���*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //����
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1);
	
	/*CAN��Ԫ����*/
	//CAN_DeInit(CAN1);
	CAN_InitStructure.CAN_TTCM = DISABLE;//��ʹ��ʱ�䴥��ģʽ
	CAN_InitStructure.CAN_ABOM = DISABLE;//��ʹ���Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;//��ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART = DISABLE;//��ʹ�ܷ��Զ��ش�ģʽ
	CAN_InitStructure.CAN_RFLM = DISABLE;//��ʹ�ܽ���FIFO����ģʽ
	CAN_InitStructure.CAN_TXFP = ENABLE;//ʹ�ܷ���FIFO���ȼ��������ɱ��ı�ʶ������
	//���������г�ʼ��
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //����ͬ����Ծ���Ϊ1��ʱ�䵥λ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //ʱ���1ռ��һ��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //ʱ���2ռ��7��ʱ�䵥λ
	CAN_InitStructure.CAN_Prescaler = 3; //��Ƶϵ��
	CAN_Init(CAN1,&CAN_InitStructure); 
	
	/*���ù�����*/
	CAN_FilterInitStructure.CAN_FilterNumber = 0; //ָ��������Ϊ0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//��ʶ���б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;//������λ��Ϊ32
	CAN_FilterInitStructure.CAN_FilterIdHigh      = 0x0000; //Ҫ���˵�ID��λ�͵�λ
	CAN_FilterInitStructure.CAN_FilterIdLow       = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh  = 0x0000; //���������α�ʶ
	CAN_FilterInitStructure.CAN_FilterMaskIdLow   = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//�趨��ָ���������FIFOΪ0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//������ʹ��
	CAN_FilterInit(&CAN_FilterInitStructure); 
	
	/*�����ж�*/
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIF00��Ϣ�Һ�����ʹ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1; //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/*дһ���жϺ��������������ݣ�����������е�while(1)��ѭ��*/
void CAN_Send_Msg(int a1,int a2,int a3,int a4)
{
	CanTxMsg txMessage;
	
	txMessage.RTR = CAN_RTR_Data; //����֡������ң��֡
	txMessage.IDE = CAN_Id_Standard; //ʹ�ñ�׼��ʶ��
	txMessage.StdId = 0x200; //��׼��ʶ�������͵ı�׼��ʶ��
	txMessage.DLC = 0x08; //��Ϣ�����ݳ���Ϊ8���ֽ�
	
	txMessage.Data[0] = (int)(a1>>8); //��Ϊ��ֵ
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




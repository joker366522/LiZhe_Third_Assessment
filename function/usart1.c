#include "main.h"

// PC12 TX  PD2 RX
// ����9���ź͵�10���Ÿ���Ϊ���ڵ���������
//�����ж����ȼ�������Խ�ӽ�0�����ȼ�Խ��
void uart5_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
    //�������ṹ�������
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//GPIOʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//����ʱ��
	//���ù���
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);  //RX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);  //TX
	
	//����GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);  
	
	//�����ж�
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//n��ʾ�ڼ��η����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; //�Ⱦ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //���
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ�ܴ����ж�
	NVIC_Init(&NVIC_InitStructure);
	
	//���ô���
	USART_InitStructure.USART_BaudRate = 115200; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //У�鷽ʽ��NO
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //ģʽ���շ��Կ�
	USART_Init(UART5,&USART_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART5,ENABLE);
}
//�жϷ����� ɾ���ͻ�����
void UART5_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(UART5,USART_FLAG_RXNE);
		ch = USART_ReceiveData(UART5);
		Packet_Decode(ch);
	}
}


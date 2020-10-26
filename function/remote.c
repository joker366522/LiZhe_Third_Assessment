#include "remote.h"

u8 sbus_rx_buffer[25];
float x_ratio,y_ratio,w_ratio;
char out;

void remote_config(void)//����2
{	
    USART_InitTypeDef usart2;
	GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1,ENABLE);//ʹ��GPIOA,DMA1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��usart2ʱ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3 ,GPIO_AF_USART2);//���ù�������
	
	gpio.GPIO_Pin = GPIO_Pin_3 ;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);
      
    USART_DeInit(USART2);
	usart2.USART_BaudRate = 100000;   //SBUS 100K baudrate ������
	usart2.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	usart2.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	usart2.USART_Parity = USART_Parity_Even;//żУ��
	usart2.USART_Mode = USART_Mode_Rx;//����ʹ��
    usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_Init(USART2,&usart2);
    USART_ClearFlag(USART2, USART_FLAG_TC);  
	USART_Cmd(USART2,ENABLE);//ʹ�ܴ���2
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);//ʹ�ܴ���2��DMA

	nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	DMA_DeInit(DMA1_Stream5);//DMA1������5
	dma.DMA_Channel= DMA_Channel_4;//DMAͨ��
	dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);//DMA�������ַ//��������Ĵ���
	dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;//DMA�ڴ����ַ
	dma.DMA_DIR = DMA_DIR_PeripheralToMemory;//���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	/*�洢�������衢���赽�洢����洢�����洢��*/
	dma.DMA_BufferSize = 18;//DMAͨ����DMA����Ĵ�С
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���ݿ��Ϊ8λ
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//���ݿ��Ϊ8λ
	dma.DMA_Mode = DMA_Mode_Circular;//������ѭ��ģʽ
	/*������Ҫ���ڴ��вɼ� 64 ���ֽڷ��͵����ڣ��������Ϊ�ظ��ɼ�����ô������ 64 ���ֽڲɼ����֮��������ڴ�ĵ�һ����ַ�ɼ������ѭ����*/
	dma.DMA_Priority = DMA_Priority_VeryHigh;//DMAͨ�� xӵ��������ȼ� 
	dma.DMA_FIFOMode = DMA_FIFOMode_Disable;//������FIFOģʽ
	dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;//ָ��FIFO����
	dma.DMA_MemoryBurst = DMA_Mode_Normal;
	dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5,&dma);

	DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);//�����ж�//DMA_IT_TC ��������ж�
	//DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5);
	DMA_Cmd(DMA1_Stream5,ENABLE);//���û����ָ����DMAy Streamx��
}

romote fpromote = 
{
	.rc.ch0 = 1024,//�ұ�����
	.rc.ch1 = 1024,//�ұ�����
	.rc.ch2 = 1024,//������� ������ת�ķ���
	.rc.ch3 = 1024,//�������
	.rc.s1 = 1,//�󿪹� ˳��1��3��2
	.rc.s2 = 3,//�ҿ���
	
	.mouse.x = 0,
	.mouse.y = 0,
	.mouse.z = 0,
	.mouse.press_l = 0,
	.mouse.press_r = 0,
	.mouse.key		 = 0
};
u8 remote_cnt = 0;
u8 systemReset_flag = 0;
romote temporary;
void DMA1_Stream5_IRQHandler(void)
{
	remote_cnt = 0;
	
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))//���ָ����DMAy Streamx�ж��Ƿ�����DMA_IT_TCIF5 Streamx��������ж�
	{
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);//���DMAͨ��x�������־λ
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);//���DMAͨ��x�жϴ������־λ
		
		temporary.rc.ch0 				= (sbus_rx_buffer[0]				| (sbus_rx_buffer[1] 	<< 8)) & 0x07ff;    // Channel 0 
		temporary.rc.ch1 				= ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] 	<< 5)) & 0x07ff;   // Channel 1         
		temporary.rc.ch2				= ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] 	<< 2)  |           // Channel 2                          
															(sbus_rx_buffer[4] << 10)) 													 & 0x07ff;  
		temporary.rc.ch3 				= ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] 	<< 7)) & 0x07ff;   // Channel 3 
		
		if ( temporary.rc.ch0 < 364 || temporary.rc.ch0 > 1684
			|| temporary.rc.ch1 < 364 || temporary.rc.ch1 > 1684
			|| temporary.rc.ch2 < 364 || temporary.rc.ch2 > 1684
			|| temporary.rc.ch3 < 364 || temporary.rc.ch3 > 1684)
		{
			fpromote.rc.ch0 = 1024;
			fpromote.rc.ch1 = 1024;     
			fpromote.rc.ch2	= 1024;                  
			fpromote.rc.ch3 = 1024;
			fpromote.rc.s1 = 1;
			fpromote.rc.s2 = 1;
			systemReset_flag = 1;
		}
		else
		{
			/* ң�������� */
			fpromote.rc.ch0 			= (sbus_rx_buffer[0]		| (sbus_rx_buffer[1] 	<< 8)) & 0x07ff;    // Channel 0 
			fpromote.rc.ch1 			= ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] 	<< 5)) & 0x07ff;   // Channel 1         
			fpromote.rc.ch2				= ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] 	<< 2)  |           // Channel 2                          
														    (sbus_rx_buffer[4] << 10)) 													 & 0x07ff;  
			fpromote.rc.ch3 			= ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] 	<< 7)) & 0x07ff;   // Channel 3 
			fpromote.rc.s1  			= ((sbus_rx_buffer[5] >> 4) & 0x000C) 					 	>> 2;
			fpromote.rc.s2 		        = ((sbus_rx_buffer[5] >> 4) & 0x0003);
			
			out = fpromote.rc.s1 ; 
			/*  ����   */
			fpromote.mouse.key 		= (sbus_rx_buffer[14] 			| (sbus_rx_buffer[15] << 8));
			for(int i = 0;i < 16;i++) 
			{
				fpromote.mouse.keys[i] = (fpromote.mouse.key >> i) & 0x01;
			}
			
			fpromote.mouse.x 			= sbus_rx_buffer[6]					| (sbus_rx_buffer[7]  << 8);
			fpromote.mouse.y 			= sbus_rx_buffer[8] 				| (sbus_rx_buffer[9]  << 8);
			fpromote.mouse.z 			= sbus_rx_buffer[10]				| (sbus_rx_buffer[11] << 8);
			fpromote.mouse.press_l = sbus_rx_buffer[12];
			fpromote.mouse.press_r = sbus_rx_buffer[13]; 

		}		
	}
}

void change(void)
{
	x_ratio=(float)((float)(fpromote.rc.ch0 - 0x400) * 2.0f / (float)0x528);
    y_ratio=(float)((float)(fpromote.rc.ch1 - 0x400) * 2.0f / (float)0x528);
	w_ratio=(float)((float)(fpromote.rc.ch2 - 0x400) * 2.0f / (float)0x528);
	
    if(x_ratio <= 0.1f && x_ratio >= -0.1f) x_ratio = 0.0f;
    if(y_ratio <= 0.1f && y_ratio >= -0.1f) y_ratio = 0.0f;
	if(w_ratio <= 0.1f && w_ratio >= -0.1f) w_ratio = 0.0f;
}



















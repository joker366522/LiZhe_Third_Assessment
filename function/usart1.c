#include "main.h"

// PC12 TX  PD2 RX
// 将第9引脚和第10引脚复用为串口的输入和输出
//设置中断优先级，数字越接近0，优先级越高
void uart5_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
    //重命名结构体的名称
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//GPIO时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//串口时钟
	//复用功能
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);  //RX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);  //TX
	
	//配置GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);  
	
	//配置中断
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//n表示第几次发生中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; //先决
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //后决
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能串口中断
	NVIC_Init(&NVIC_InitStructure);
	
	//配置串口
	USART_InitStructure.USART_BaudRate = 115200; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //校验方式：NO
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //不设置硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //模式：收发皆可
	USART_Init(UART5,&USART_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART5,ENABLE);
}
//中断服务函数 删除就会死机
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


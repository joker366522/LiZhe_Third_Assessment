#ifndef _CAN_H
#define _CAN_H

#include "main.h"
typedef struct
{
	float Speed_wheel[4],Mechanical_wheel[4];
	float Mechanical_Yaw_Angle,Mechanical_Pitch_Angle,Mechanical_Roll_Angle;
	float Speed_Yaw_Angle,Speed_Pitch_Angle,Speed_Roll_Angle;
	float torque_current_wheel[7],temperature[7]; //扭矩、温度
	uint8_t flag;
	float LF_current,RF_current,LR_current,RR_current;
}motor; //motor为抽象数据类型

extern motor fpchassis;
extern void CAN1_Mode_Init(void);
extern void CAN2_Mode_Init(void);

extern void CAN1_RX0_IRQHandler(void);
extern void CAN1_Send_Msg(int a1,int a2,int a3,int a4);
extern void CAN2_Send_Msg(int a,int b);

#endif

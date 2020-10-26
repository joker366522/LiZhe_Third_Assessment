#ifndef _TIMER_H
#define _TIMER_H

#include "main.h"

extern void TIM3_Int_Init(void);
extern void TIM3_IRQHandler(void);

extern float Eular[3];
extern short Gyo[3];
extern float w1_ratio;
#endif


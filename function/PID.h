#ifndef _PID_H
#define _PID_H

#include "main.h"

extern void PidInit(void);
extern float PidCalculation1(void);
extern float PidCalculation2(void);
extern float PidCalculation3(void);
extern float PidCalculation4(void);
extern float PidCalculation_UP12(void);
extern float PidCalculation_UP122(void);

extern float Circle(void);

#endif


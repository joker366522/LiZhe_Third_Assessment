#ifndef _REMOTE_H_
#define _REMOTE_H_
#include "main.h"
typedef struct
{
	struct
	{
		int16_t ch0,ch1,ch2,ch3;
		u8 s1,s2;
	}rc;
		
	struct
	{	
		int16_t x,y,z,key;
		int8_t keys[16];
		uint8_t press_l,press_r;
	}mouse;
	
}romote;
extern romote fpromote;
extern u8 remote_cnt;
extern u8 systemReset_flag;

extern float x_ratio;
extern float y_ratio;
extern float w_ratio;

extern char out;

extern void change(void);
extern float StraightCalculation(void);

extern void StraightInit(void);
extern float StraightCalculation(void);

extern void remote_config(void);


#endif


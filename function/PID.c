#include "main.h"
typedef struct 
{
    float Kp,Ki,Kd; 

	float want_speed;   
	float now_speed;
	
	float now_error;
	float last_error; //上次误差
	float previous_error; //上上次误差
	float i_error; //位置式PID独有的误差和
	float sum_error;
	float result;
	
	float out_max;
	float limit;
}PID_Structure;

PID_Structure Error1,Error2,Error3,Error4;

void PidInit(void)
{
	Error1.Kp = Error2.Kp = Error3.Kp = Error4.Kp = 10;
	Error1.Ki = Error2.Ki = Error3.Ki = Error4.Ki = 0.01;
	Error1.Kd = Error2.Kd = Error3.Kd = Error4.Kd = 0;
	
	Error1.want_speed = Error2.want_speed = Error3.want_speed = Error4.want_speed =0;
	Error1.now_speed = Error2.now_speed = Error3.now_speed = Error4.now_speed = 0;
	
	Error1.now_error = Error2.now_error = Error3.now_error = Error4.now_error =0;
	Error1.last_error = Error2.last_error = Error3.last_error = Error4.last_error =0;
	Error1.previous_error = Error2.previous_error = Error3.previous_error = Error4.previous_error = 0;
	Error1.i_error = Error2.i_error = Error3.i_error = Error4.i_error = 0;
	
	Error1.out_max = Error2.out_max = Error3.out_max = Error4.out_max = 8000;
	Error1.limit = Error2.limit = Error3.limit = Error4.limit = 3000;
	Error1.result = Error2.result = Error3.result = Error4.result = 0;
}

float PidCalculation1(void)
{
	Error1.want_speed = -y_ratio*Error1.limit - x_ratio*Error1.limit + w_ratio*Error1.limit;
	Error1.now_speed = fpchassis.Speed_wheel[0];
	Error1.now_error = Error1.want_speed - Error1.now_speed;
	Error1.i_error = Error1.now_error * Error1.Ki;
	
	Error1.result += Error1.Kp * (Error1.now_error-Error1.last_error) + Error1.i_error + Error1.Kd * (Error1.now_error-2*Error1.last_error+Error1.previous_error);

	Error1.previous_error = Error1.last_error;
	Error1.last_error = Error1.now_error;
	//Error.result = Error.Kp * Error.now_error + Error.Ki * Error.now_error + Error.Kd * (Error.now_error-Error.last_error);
	Error1.result = Error1.result > Error1.out_max ? Error1.out_max : Error1.result;
	Error1.result = Error1.result < -Error1.out_max ? -Error1.out_max : Error1.result;
	return Error1.result;
}

float PidCalculation2(void)
{
	Error2.want_speed = -y_ratio*Error2.limit + x_ratio*Error2.limit + w_ratio*Error2.limit;
	Error2.now_speed = fpchassis.Speed_wheel[1];
	Error2.now_error = Error2.want_speed - Error2.now_speed;
	Error2.i_error = Error2.now_error * Error1.Ki;
	
	Error2.result += Error2.Kp * (Error2.now_error-Error2.last_error) + Error2.i_error + Error2.Kd * (Error2.now_error-2*Error2.last_error+Error2.previous_error);

	Error2.previous_error = Error2.last_error;
	Error2.last_error = Error2.now_error;
	//Error.result = Error.Kp * Error.now_error + Error.Ki * Error.now_error + Error.Kd * (Error.now_error-Error.last_error);
	Error2.result = Error2.result > Error2.out_max ? Error2.out_max : Error2.result;
	Error2.result = Error2.result < -Error2.out_max ? -Error2.out_max : Error2.result;
	return Error2.result;
}

float PidCalculation3(void)
{
	Error3.want_speed = y_ratio*Error3.limit - x_ratio*Error3.limit + w_ratio*Error3.limit;
	Error3.now_speed = fpchassis.Speed_wheel[2];
	Error3.now_error = Error3.want_speed - Error3.now_speed;
	Error3.i_error = Error3.now_error * Error3.Ki;
	
	Error3.result += Error3.Kp * (Error3.now_error-Error3.last_error) + Error3.i_error + Error3.Kd * (Error3.now_error-2*Error3.last_error+Error3.previous_error);

	Error3.previous_error = Error3.last_error;
	Error3.last_error = Error3.now_error;
	//Error.result = Error.Kp * Error.now_error + Error.Ki * Error.now_error + Error.Kd * (Error.now_error-Error.last_error);
	Error3.result = Error3.result > Error3.out_max ? Error3.out_max : Error3.result;
	Error3.result = Error3.result < -Error3.out_max ? -Error3.out_max : Error3.result;
	return Error3.result;
}

float PidCalculation4(void)
{
	Error4.want_speed = y_ratio*Error4.limit + x_ratio*Error4.limit +w_ratio*Error4.limit;
	Error4.now_speed = fpchassis.Speed_wheel[3];
	Error4.now_error = Error4.want_speed - Error4.now_speed;
	Error4.i_error = Error4.now_error * Error4.Ki;
	
	Error4.result += Error4.Kp * (Error4.now_error-Error4.last_error) + Error4.i_error + Error4.Kd * (Error4.now_error-2*Error4.last_error+Error4.previous_error);

	Error4.previous_error = Error4.last_error;
	Error4.last_error = Error4.now_error;
	//Error.result = Error.Kp * Error.now_error + Error.Ki * Error.now_error + Error.Kd * (Error.now_error-Error.last_error);
	Error4.result = Error4.result > Error4.out_max ? Error4.out_max : Error4.result;
	Error4.result = Error4.result < -Error4.out_max ? -Error4.out_max : Error4.result;
	return Error4.result;
}

/*
float PID_Increment(PID *pid,float tar,float speed_rx)
{
	pid->tar = tar;//目标值
	pid->cur = speed_rx;//实际值
	pid->thisError = pid->tar - pid->cur;//现误差
	//pid->thisError =  pid->cur - pid->tar;	//现误差
	
	pid->pError = pid->proportiongain * (pid->thisError - pid->lastError);//P*（现误差-上一次误差）
		
	pid->iError = pid->integralgain * pid->thisError;//I*现误差
	
	pid->dError = (pid->thisError - 2*pid->lastError + pid->beforeError)*pid->differential;//D*(现误差-2*上次误差+上上次误差)
	
	pid->result += pid->pError + pid->iError + pid->dError;//我
	
	pid->beforeError = pid->lastError;//保留上次误差
	pid->lastError = pid->thisError;//保留现误差
	
	pid->result = pid->result > pid->out_max ? pid->out_max : pid->result;
	pid->result = pid->result < -pid->out_max ? -pid->out_max : pid->result;
	return pid->result;
}
*/

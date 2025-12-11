#ifndef _SELF_PID_H_
#define _SELF_PID_H_

#include "Self_BSP.h"

typedef struct 
{
    float target_val;   //目标值
    float actual_val;   //实际值
    float err;          //定义当前偏差值
    float err_next;     //定义下一个偏差值
    float err_last;     //定义最后一个偏差值
    float Kp,Ki,Kd;     //定义比例·积分·微分函数
}_pid;

extern _pid Servo_pid;
extern _pid Turn_Pid_L;
extern _pid Turn_Pid_R;
extern _pid Right_Motor_pid;
extern _pid Left_Motor_pid;

//初始化代码
void PID_Servo_init(void);
void PID_Right_Motor_init(void);
void PID_Left_Motor_init(void);
void Turn_Right_Init(void);

void set_pid_Servo_target(float temp_val);
void set_Servo_p_i_d(float p, float i, float d);
float PID_Servo_realize(float actual_val);
void set_pid_Right_Motor_target(float temp_val);
void set_pid_Left_Motor_target(float temp_val);

#endif // DEBUG

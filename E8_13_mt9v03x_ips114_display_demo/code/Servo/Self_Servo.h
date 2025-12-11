#ifndef _SELF_SERVO_H_
#define _SELF_SERVO_H_

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                       // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MOTOR_L_MAX           (50 )                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (150)                                       // 定义主板上舵机活动范围 角度

#define S_PWM_Min 300   //最小舵机值
#define S_PWM_Max 730   //最大舵机值
#define S_PWM_Mid 515   //中间舵机值
#define S_PWM_H   215.0   //舵机半程差

#include "Self_BSP.h"

void self_servo_init(void);
void self_servo_process(void);


#endif // DEBUG
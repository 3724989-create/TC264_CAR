#ifndef _SELF_MOTOR_H_
#define _SELF_MOTOR_H_

#include "Self_BSP.h"

#define MAX_DUTY            (50 )                                               // 最大 MAX_DUTY% 占空比
#define DIR_R1              (P02_4)
#define PWM_R1              (ATOM0_CH5_P02_5)
#define DIR_L1              (P02_6)
#define PWM_L1              (ATOM0_CH7_P02_7)

#define DIR_R2              (P21_2)
#define PWM_R2              (ATOM0_CH1_P21_3)
#define DIR_L2              (P21_4)
#define PWM_L2              (ATOM0_CH3_P21_5)

//元素枚举
typedef enum
{
    //无
    Element_Null,
    //圆环
    Circle_LeftSide,
    Circle_RightSide,
    //三岔
    ThreeFork,
    //十字
    Centre_Cross,
    Left_Cross,
    Right_Cross,
    //断路
    Open_Circuit,
    //路障
    Left_Block,
    Right_Block,
    //车库
    Garage,
    Garage_Out,
    //坡道
    Ramp

}Element_enum;

//锁定枚举
typedef enum
{
    UnLock,
    Lock
}Lock_enum;

//元素结构体
typedef struct
{
    Element_enum Name;
    Lock_enum Lock;
    uint8 State;
}Element_Type;

void self_motor_init(void);
void self_motor_process(void);

#endif // DEBUG
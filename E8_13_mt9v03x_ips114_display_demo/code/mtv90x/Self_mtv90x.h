#ifndef _SELF_mtv90x_H_
#define _SELF_mtv90x_H_

#include "Self_BSP.h"

extern float up_long_sum,up_long_num_sum;
extern uint8_t Realwind;
extern float control_error;
extern int Control_Mid;

// ===================================
// Element Structure and Enumerations
// 赛道元素结构体和枚举定义
// ===================================

// 元素枚举 (Element_enum)
typedef enum
{
    // 无元素/默认状态
    Element_Null,

    // 圆环/环岛
    Circle_LeftSide,    // 左侧圆环
    Circle_RightSide,   // 右侧圆环

    // 三岔路
    ThreeFork,          // 三岔路口

    // 十字
    Centre_Cross,       // 中间十字路口
    Left_Cross,         // 左入十字路口
    Right_Cross,        // 右入十字路口

    // 断路
    Open_Circuit,       // 断路/缺口

    // 路障
    Left_Block,         // 左侧路障
    Right_Block,        // 右侧路障

    // 车库
    Garage,             // 入库
    Garage_Out,         // 出库

    // 坡道
    Ramp                // 坡道

}Element_enum;


// 锁定状态枚举 (Lock_enum)
typedef enum
{
    UnLock,     // 未锁定/未发现
    Lock        // 已锁定/已发现

}Lock_enum;


// 元素结构体 (Element_Type)
typedef struct
{
    Element_enum Name;  // 元素名称 (Element_enum 类型)
    Lock_enum Lock;     // 锁定状态 (Lock_enum 类型, 用于防止重复触发或干扰)
    uint8_t State;      // 元素阶段/状态机 (例如：坡道 State 1=上坡, 2=坡中)

}Element_Type;

extern Element_Type Element_Circle_LeftSide;  //左圆环
extern Element_Type Element_Circle_RightSide; //右圆环
extern Element_Type Element_Centre_Cross;     //中十字
extern Element_Type Element_Left_Cross;       //左入十字
extern Element_Type Element_Right_Cross;      //右入十字
extern Element_Type Element_Right_Block;     //右路障
extern Element_Type Element_Left_Block;     //路障
extern Element_Type Element_Garage;           //车库
extern Element_Type Element_Garage_Out;       //出库
extern Element_Type Element_Ramp;             //坡道
//打脚行确定看哪里，而误差值确定转向程度

void self_mtv90x_init(void);
void self_mtv90x_process(void);


#endif // DEBUG

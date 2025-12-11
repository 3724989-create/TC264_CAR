#ifndef _VC_FUZZY_H_
#define _VC_FUZZY_H_

// 论域指的是一个变量所有可能取值的集合或范围。

#include "Self_BSP.h"

// 外部声明模糊规则表
extern float rule_single_sp[7][7];
extern float rule_double_im[7][7];

// 外部声明输入论域（模糊集分割点）
//extern float P_Speed_value[7];
//extern float P_Realwind_value[7];
//extern float D_Speed_value[7];

// 函数声明：双向模糊控制函数
float Fuzzy_double(float *PFF_double,float *DFF_double,float UFF_double,float *rule_double,float P,float D);
// 函数声明：单向模糊控制函数
float Fuzzy_single(float *PFF_single,float *DFF_single,float UFF_single,float *rule_single,float P,float D) ;


#endif // _VC_FUZZY_H_

#ifndef _SELF_MATH_H_
#define _SELF_MATH_H_

#include "Self_BSP.h"

extern float Regression_B_M;
extern float Regression_K_M;
extern float Regression_B_L;
extern float Regression_K_L;
extern float Regression_B_R;
extern float Regression_K_R;
extern float Regression_B_U;
extern float Regression_K_U;

uint8_t Limit_ab_uint8(uint8_t x,uint8_t a,uint8_t b);
int Limit_ab_int(int x,int a,int b);
float Limit_ab_float(float x,float a,float b);
int Regression_now(int startline,int endline,int type);
float Variance_Count_Now(uint8_t startline,uint8_t endline,uint8_t type);


#endif // DEBUG


#include "self_math.h"

uint8_t Limit_ab_uint8(uint8_t x,uint8_t a,uint8_t b)
{
    if(x<a)return a;
    if(x>b)return b;
    return x;
}


int Limit_ab_int(int x,int a,int b)
{
    if(x<a)return a;
    if(x>b)return b;
    return x;
}

float Limit_ab_float(float x,float a,float b)
{
    if(x<a)return a;
    if(x>b)return b;
    return x;
}
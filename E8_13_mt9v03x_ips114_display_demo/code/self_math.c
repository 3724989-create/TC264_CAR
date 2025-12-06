
#include "self_math.h"

uint8 Limit_ab_uint8(uint8 x,uint8 a,uint8 b)
{
    if(x<a)return a;
    if(x>b)return b;
    return x;
}
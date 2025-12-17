
#include "self_math.h"

float Regression_B_M;
float Regression_K_M;
float Regression_B_L;
float Regression_K_L;
float Regression_B_R;
float Regression_K_R;
float Regression_B_U;
float Regression_K_U;
float Regression_B_L_P;//逆透视左线截距
float Regression_K_L_P;//逆透视左线斜率
float Regression_B_R_P;//逆透视右线截距
float Regression_K_R_P;//逆透视右线斜率

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

//-------------------------------------------------------------------------------------------------------------------
//  @brief      最小二乘法拟合直线
//  @param      type             选择计算左/中/右线  0左 2右 1中 3上
//  @param      startline        起始行
//  @param      endline          结束行
//  @return     null
//-------------------------------------------------------------------------------------------------------------------

int Regression_now(int startline,int endline,int type)
{
    int i = 0;
    int sumlines = 0;
    float sumX = 0;
    float sumY = 0;
    float averageX = 0;
    float averageY = 0;
    float sumUp = 0;
    float sumDown = 0;

    if(type==0)
    {
        for(i=endline;i>startline;i--)
        {
            if(LeftLine[1]>184)break;
            sumlines++; //统计有效点的数量
            sumX+=i;
            sumY+=LeftLine[i];
        }
        if(sumlines==0)sumlines=1;
        averageX=sumX/sumlines;
        averageY=sumY/sumlines;

        for(i=endline;i>endline-sumlines;i--)   //184以上的点
        {
            sumUp+=(LeftLine[i]-averageY)*(i-averageX);
            sumDown+= (i-averageX)*(i-averageX);
        }
        if(sumDown==0)Regression_K_L=sumUp/sumDown;
        Regression_B_L=averageY-Regression_K_L*averageX;

    }
    else if (type == 2)
    {
        for (i = endline; i >startline ; i--)
        {

//            if(Right[i]<4||(Right[i]<30 && Right[i]==Right[i-1] && Right[i-1]==Right[i-2])) break;
            if(RightLine[i]<4) break;//||(Pixels2[i*MT9V03X_W+Right[i]+1]==0x0000 && Pixels2[i*MT9V03X_W+Right[i]-1]==0x0000&& Right[i]==Right[i-1] && Right[i-1]==Right[i-2]
            sumlines++;
            sumX += i;
            sumY += RightLine[i];

        }
        if (sumlines == 0) sumlines = 1;
        averageX = sumX / sumlines;
        averageY = sumY / sumlines;

        for (i = endline; i > endline-sumlines; i--)
        {
            sumUp += (RightLine[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) Regression_K_R = 0;
        else Regression_K_R = sumUp / sumDown;
        Regression_B_R = averageY - Regression_K_R * averageX;
    }
     else if (type == 1)
    {
        for (i = endline; i >startline ; i--)
       {
           if(MidLine[i]<4||MidLine[i]>184 ) break;//||&& Pixels2[i*MT9V03X_W+Right[i]+1]==0x0000 && Pixels2[i*MT9V03X_W+Right[i]-1]==0x0000)(New_LR_Center[i]==New_LR_Center[i-1] && New_LR_Center[i-1]==New_LR_Center[i-2]
           sumlines++;
           sumX += i;
           sumY += MidLine[i];
       }
       if (sumlines != 0)
       {
           averageX = sumX / sumlines;
           averageY = sumY / sumlines;
       }
       else
       {
           averageX = 0;
           averageY = 0;
       }

       for (i = endline; i > endline-sumlines; i--)
       {
           sumUp += (MidLine[i] - averageY) * (i - averageX);
           sumDown += (i - averageX) * (i - averageX);
       }
       if (sumDown == 0) Regression_K_M = 0;
       else Regression_K_M = sumUp / sumDown;
       Regression_B_M = averageY - Regression_K_M * averageX;
   }
     else if (type == 3)
    {
        for (i = endline; i >startline ; i--)
        {
            sumlines++;
            sumX += i;
            sumY += Up[i];
        }
        if (sumlines == 0) sumlines = 1;
        averageX = sumX / sumlines;
        averageY = sumY / sumlines;

        for (i = endline; i > endline-sumlines; i--)
        {
            sumUp += (Up[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) Regression_K_U = 0;
        else Regression_K_U = sumUp / sumDown;
        Regression_B_U = averageY - Regression_K_U * averageX;
   }
   return sumlines;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      方差统计
//  @param      type             选择计算左/中/右线  0左1中 2右
//  @param      startline        起始行
//  @param      endline          结束行
//  @return     null
//-------------------------------------------------------------------------------------------------------------------

float Variance_Count_Now(uint8_t startline,uint8_t endline,uint8_t type)
{
    uint8_t i = 0;
    float sum = 0;
    int sumlines = 0;
    float Variance_M;
    float Variance_L;
    float Variance_R;
    float Variance_U;
    sumlines=Regression_now(startline, endline, type);  //统计有效点
    if(0==type)
    {
        for(i=endline;i>endline-sumlines;i--)
        {
          sum += (LeftLine[i] - (i * Regression_K_L + Regression_B_L)) *
          (LeftLine[i] - (i * Regression_K_L + Regression_B_L));    //左右线和拟合直线做方差
        }
        Variance_L = sum / sumlines;
        return Variance_L;
    }
    else if (1 == type)
    {
        for (i = endline; i > endline-sumlines; i--)
      {
          sum += (MidLine[i] - (i * Regression_K_M + Regression_B_M))*
                 (MidLine[i] - (i * Regression_K_M + Regression_B_M));
      }
      Variance_M = sum / sumlines;

      return Variance_M;
    }
    else if (2 == type)
    {
        for (i = endline; i > endline-sumlines; i--)
        {
            sum += (RightLine[i] - (i * Regression_K_R + Regression_B_R)) *
                   (RightLine[i] - (i * Regression_K_R + Regression_B_R));
        }
        Variance_R = sum / sumlines;

        return Variance_R;
    }
    else if (3 == type)
    {
        for (i = endline; i > endline-sumlines; i--)
        {
            sum += (Up[i] - (i * Regression_K_U + Regression_B_U)) *
                   (Up[i] - (i * Regression_K_U + Regression_B_U));
        }
        Variance_U = sum / sumlines;

        return Variance_U;
    }

    return 0;
}
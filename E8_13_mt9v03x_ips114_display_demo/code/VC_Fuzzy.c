#include "VC_Fuzzy.h"


#define PMAX  100
#define PMIN -100
#define DMAX  100
#define DMIN -100
#define FMAX  100       //语言值的满幅值

#define LISTLINGTH  7   //表长
/*******************************双向模糊**********************************/
float PFF_double_im[4] = { 0, 0.15, 0.5847, 1.1695 }; //偏差 //0 10 30 60同时定义了-10 -30 -60三个定义状态
float DFF_double_im[4] = { 0, 0.0839, 0.1679, 0.2519 }; //偏差微分 //0 30 60 90,指变化率

float UFF_double_im = 200; //舵机输出值

////双向舵机模糊规则表
float rule_double_im[7][7] = {
    /*EC*/
    /*0-6*/  {-100, -100, -80, -80, -60, -20, -20,},
    /*7-13*/ {-100, -80, -80, -80, -80, -40, -40,},
    /*14-20*/{ -60, -60, -40,  0,  0,  20, -40,},
    /*21-27*/{ -40, -60,  0,  0,  0,  60,  40,},
    /*28-34*/{  40, -20,  0,  0, 40,  80,  80,},
    /*35-41*/{  40,  40, 80, 80, 80,  80, 100,},
    /*42-48*/{  20,  20, 60, 80, 80, 100, 100,}
};

//模糊规则表
//int16 rule[7][7] = {//误差变化率 -3,-2,-1, 0, 1, 2, 3                //   误差
//         /*0-6*/{-6,-6,-6,-6,-5,-5,-4,},                        //   -3      0
//        /*7-13*/{-5,-5,-5,-4,-4,-3,-1,},                        //   -2      1
//                {-4,-4,-3,-1, 0, 1, 2,},                        //   -1      2
//       /*21-27*/{-4,-3,-1, 0, 1, 3, 4,},                        //    0      3
//                {-2,-1, 0, 1, 3, 4, 4,},                        //    1      4
//                { 1, 3, 4, 4, 5, 5, 5,},                        //    2      5
//                { 4, 5, 5, 6, 6, 6, 6,} };                      //    3     6  5.94

/*******************************Tips***********************************/
//在智能车实际跑动时, 若希望提前打角更大, 则增大模糊规则表
//左上角和右下角区域的隶属值 若希望在弯道中切内, 则增大模糊规则表左侧和右
//侧的隶属值 若发现出弯时智能车抖动严重, 则调小模糊规则表左下角和右上
//角的隶属值; 若发现智能车在直道中摆动严重, 则调小模糊规则表中间区域的
//隶属值. 需要特别注意的是, 在保证智能车的机械结构对称的前提下, 模糊规
//则表一定是关于中心对称的;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      双边模糊
//  @param      *PFF_double   竖轴输入论域
//  @param      *DFF_double   横轴输入论域
//  @param      UFF_double    输出最大值
//  @param      *rule_double  模糊表
//  @param      P             竖轴输入
//  @param      D             横轴输入
//  @return     U             输出值
//  Sample usage:   Fuzzy_double(PFF_double_im, DFF_double_im, UFF_double_im, rule_double_im[0], p, d) ;
//-------------------------------------------------------------------------------------------------------------------
float Fuzzy_double(float *PFF_double,float *DFF_double,float UFF_double,float *rule_double,float P,float D){
    int Pn,Dn;
    int Un[4];
    float U_p,U;        //输出值的精确量
    float PF[2],DF[2];  //输出值的隶属度
    /*隶属度的确定*/
    //0为左，上隶属,NB到NM的过渡区。-2 -1 0，1 2 3
    if (P > -PFF_double[3] && P < PFF_double[3]) {
        if (P <= -PFF_double[2]) {
            Pn = -2;
            PF[0] = FMAX * ((float)(-PFF_double[2] - P) / (PFF_double[3] - PFF_double[2]));
        } else if (P <= -PFF_double[1]) {
            Pn = -1;
            PF[0] = FMAX * ((float)(-PFF_double[1] - P) / (PFF_double[2] - PFF_double[1]));
        } else if (P <= PFF_double[0]) {
            Pn = 0;
            PF[0] = FMAX * ((float)(-PFF_double[0] - P) / (PFF_double[1] - PFF_double[0]));
        } else if (P <= PFF_double[1]) {
            Pn = 1;
            PF[0] = FMAX * ((float)(PFF_double[1] - P) / (PFF_double[1] - PFF_double[0]));
        } else if (P <= PFF_double[2]) {
            Pn = 2;
            PF[0] = FMAX * ((float)(PFF_double[2] - P) / (PFF_double[2] - PFF_double[1]));
        } else if (P <= PFF_double[3]) {
            Pn = 3;
            PF[0] = FMAX * ((float)(PFF_double[3] - P) / (PFF_double[3] - PFF_double[2]));
        }

    } else if (P <= -PFF_double[3]) {
        Pn = -2;
        PF[0] = FMAX;

    } else if (P >= PFF_double[3]) {
        Pn = 3;
        PF[0] = 0;
    }

    PF[1] = FMAX - PF[0];

        if (D > -DFF_double[3] && D < DFF_double[3]) {
        if (D <= -DFF_double[2]) {
            Dn = -2;
            DF[0] = DMAX * ((float)(-DFF_double[2] - D) / (DFF_double[3] - DFF_double[2]));
        } else if (D <= -DFF_double[1]) {
            Dn = -1;
            DF[0] = DMAX * ((float)(-DFF_double[1] - D) / (DFF_double[2] - DFF_double[1]));
        } else if (D <= DFF_double[0]) {
            Dn = 0;
            DF[0] = DMAX * ((float)(-DFF_double[0] - D) / (DFF_double[1] - DFF_double[0]));
        } else if (D <= DFF_double[1]) {
            Dn = 1;
            DF[0] = DMAX * ((float)(DFF_double[1] - D) / (DFF_double[1] - DFF_double[0]));
        } else if (D <= DFF_double[2]) {
            Dn = 2;
            DF[0] = DMAX * ((float)(DFF_double[2] - D) / (DFF_double[2] - DFF_double[1]));
        } else if (D <= DFF_double[3]) {
            Dn = 3;
            DF[0] = DMAX * ((float)(DFF_double[3] - D) / (DFF_double[3] - DFF_double[2]));
        }

    } else if (D <= -DFF_double[3]) {
        Dn = -2;
        DF[0] = DMAX;

    } else if (D >= DFF_double[3]) {
        Dn = 3;
        DF[0] = 0;
    }

     DF[1] = DMAX - DF[0];

     // (Pn - 1 + 3) * LISTLINGTH表示行移动量(Dn + 3)表示列移动量，最后锁定四个值
    Un[0] = *(rule_double + (Pn - 1 + 3) * LISTLINGTH + (Dn - 1 + 3));//左上//PN-1因为之前PN最小只等于-2
    Un[1] = *(rule_double + (Pn  + 3) * LISTLINGTH + (Dn - 1 + 3));//左下
    Un[2] = *(rule_double + (Pn - 1 + 3) * LISTLINGTH + (Dn + 3));//右上
    Un[3] = *(rule_double + (Pn + 3) * LISTLINGTH + (Dn + 3));//右下

    //求输出量化值，通过加权平均使其变得平滑
    U_p = (Un[0] * DF[0] * PF[0]
         + Un[1] * DF[0] * PF[1]
         + Un[2] * DF[1] * PF[0]
         + Un[3] * DF[1] * PF[1]) / 1000000;

    U = U_p * UFF_double;


    return U;
     

}

/*******************************单向模糊**********************************/
//输入要从小到大
//P是竖轴，D是
float PFF_single_im[7] = { 0, 1, 2, 3, 4, 5, 6};  //偏差 //0 10 30 60
float DFF_single_im[7] = { 0, 1, 2, 3, 4, 5, 6};  //偏差微分 //0 30 60 90

float UFF_single_im = 200; //舵机输出值

////单向舵机模糊规则表
float rule_single_im[7][7] = {
    /*EC*/
    /*0-6*/  { 0, 10, 20, 30, 40, 50, 60,},
    /*7-13*/ { 10, 20, 30, 40, 50, 60, 70,},
    /*14-20*/{ 20, 30, 40, 50, 60, 70, 80,},
    /*21-27*/{ 30, 40, 50, 60, 70, 80, 90,},
    /*28-34*/{ 40, 50, 60, 70, 80, 90, 100,},
    /*35-41*/{ 50, 60, 70, 80, 90, 100, 100,},
    /*42-48*/{ 60, 70, 80, 90, 100, 100, 100,}
};

float P_Up_Long_Line_value[7] = { 6, 11, 16, 21, 26, 31, 36};  //上线论域
float D_LR_error_value[7] = { 0, 15.5, 31, 46.5, 62, 77.5, 93};//左右论域

float U_Cen_Buff = 1.8;  //中线增益输出值

////中线增益表
float U_Cen_Buff_list[7][7] = {
    /*EC*/ //0 ->
    /*0-6*/  { 30, 30, 30, 30, 30, 30, 30,},
    /*7-13*/ { 40, 40, 40, 40, 40, 40, 40,},
    /*14-20*/{ 50, 50, 50, 50, 50, 50, 50,},
    /*21-27*/{ 60, 60, 60, 60, 60, 60, 60,},
    /*28-34*/{ 70, 70, 70, 70, 70, 70, 70,},
    /*35-41*/{ 95, 95, 95, 95, 95, 95, 95,},
    /*42-48*/{ 100, 100, 100, 100, 100, 100, 100,}
};


//////单向舵机模糊规则表
//float rule_single_im[7][7] = {
//    //横坐标偏差，纵坐标速度
//    { 0, 10, 20, 30, 40, 50, 60,},
//    { 10, 20, 30, 40, 50, 60, 70,},
//    { 20, 30, 40, 50, 60, 70, 80,},
//    { 30, 40, 50, 60, 70, 80, 90,},
//    { 40, 50, 60, 70, 80, 90, 100,},
//    { 50, 60, 70, 80, 90, 100, 100,},
//    { 60, 70, 80, 90, 100, 100, 100,}
//};


float P_Speed_value[7] = {  15, 18,  20, 22, 24, 25 ,27};      //上线论域
float P_Realwind_value[7]={  15, 18,  20, 22, 24, 25 ,27};   //打脚行上线论域
float D_Speed_value[7] = {  0, 4,6,8,12,15,18};//偏差变化论域

float U_Speed_Buff = 1.8;  //中线增益输出值
//////单向速度决策模糊规则表
//float rule_single_sp[7][7] = {
//    //横坐标上扫线，纵坐标偏差变化
//    { 100, 100, 96, 92,88, 84,80,},
//    { 80, 80, 76, 72,68, 64,60,},
//    { 60, 60, 56, 52,48, 44,40,},
//    { 40, 40, 38, 36,34, 32,30,},
//    { 30, 30, 28, 26,24, 22,20,},
//    { 20, 20, 18, 16,14, 12,10,},
//    { 10, 10, 8, 6,4, 2, 0,}
//};

float rule_single_sp[7][7] = {
    //横坐标上扫线，纵坐标偏差变化
    { 100, 100, 96, 92,88, 84,80,},
    { 70, 80, 76, 72,68, 64,60,},
    { 50, 60, 56, 52,48, 44,40,},
    { 30, 40, 38, 36,34, 32,30,},
    { 15, 30, 28, 26,24, 22,20,},
    { 5, 20, 18, 16,14, 12,10,},
    { 0, 10, 8, 6,4, 2, 0,}
};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      单边模糊
//  @param      *PFF_single   竖轴输入论域
//  @param      *DFF_single   横轴输入论域
//  @param      UFF_single    输出最大值
//  @param      *rule_single  模糊表
//  @param      P             竖轴输入
//  @param      D             横轴输入
//  @return     U             输出值
//  Sample usage:   Fuzzy_single(PFF_single_im, DFF_single_im, UFF_single_im, rule_single_im[0], p, d) ;
//-------------------------------------------------------------------------------------------------------------------
//原理和双向一样不过没有负号
float Fuzzy_single(float *PFF_single, float *DFF_single, float UFF_single, float *rule_single, float P, float D) {
    int Pn, Dn;
    int Un[4];
    float U_p,U;       //输出值的精确量
    float PF[2], DF[2];//偏差,偏差微分以及输出值的隶属度


    /*隶属度的确定*/
    //0为左，上，隶属
    if (P > PFF_single[0] && P < PFF_single[6]) {
        if (P <= PFF_single[1]) {
            Pn = 1;
            PF[0] = FMAX * ((float)(PFF_single[1] - P) / (PFF_single[1] - PFF_single[0]));
        } else if (P <= PFF_single[2]) {
            Pn = 2;
            PF[0] = FMAX * ((float)(PFF_single[2] - P) / (PFF_single[2] - PFF_single[1]));
        } else if (P <= PFF_single[3]) {
            Pn = 3;
            PF[0] = FMAX * ((float)(PFF_single[3] - P) / (PFF_single[3] - PFF_single[2]));
        } else if (P <= PFF_single[4]) {
            Pn = 4;
            PF[0] = FMAX * ((float)(PFF_single[4] - P) / (PFF_single[4] - PFF_single[3]));
        } else if (P <= PFF_single[5]) {
            Pn = 5;
            PF[0] = FMAX * ((float)(PFF_single[5] - P) / (PFF_single[5] - PFF_single[4]));
        } else if (P <= PFF_single[6]) {
            Pn = 6;
            PF[0] = FMAX * ((float)(PFF_single[6] - P) / (PFF_single[6] - PFF_single[5]));
        }


    } else if (P <= PFF_single[0]) {
        Pn = 1;
        PF[0] = FMAX;


    } else if (P >= PFF_single[6]) {
        Pn = 6;
        PF[0] = 0;
    }

    PF[1] = FMAX - PF[0];


    if (D > DFF_single[0] && D < DFF_single[6]) {
        if (D <= DFF_single[1]) {
            Dn = 1;
            DF[0] = DMAX * ((float)(DFF_single[1] - D) / (DFF_single[1] - DFF_single[0]));
        } else if (D <= DFF_single[2]) {
            Dn = 2;
            DF[0] = DMAX * ((float)(DFF_single[2] - D) / (DFF_single[2] - DFF_single[1]));
        } else if (D <= DFF_single[3]) {
            Dn = 3;
            DF[0] = DMAX * ((float)(DFF_single[3] - D) / (DFF_single[3] - DFF_single[2]));
        } else if (D <= DFF_single[4]) {
            Dn = 4;
            DF[0] = DMAX * ((float)(DFF_single[4] - D) / (DFF_single[4] - DFF_single[3]));
        } else if (D <= DFF_single[5]) {
            Dn = 5;
            DF[0] = DMAX * ((float)(DFF_single[5] - D) / (DFF_single[5] - DFF_single[4]));
        } else if (D <= DFF_single[6]) {
            Dn = 6;
            DF[0] = DMAX * ((float)(DFF_single[6] - D) / (DFF_single[6] - DFF_single[5]));
        }


    } else if (D <= DFF_single[0]) {
        Dn = 1;
        DF[0] = DMAX;


    } else if (D >= DFF_single[6]) {
        Dn = 6;
        DF[0] = 0;
    }

    DF[1] = DMAX - DF[0];

    //得模糊表四个值

//  Un[0] = rule_double[Pn - 1 + 3][Dn - 1 + 3];//左上
//  Un[1] = rule_double[Pn + 3][Dn - 1 + 3];    //左下
//  Un[2] = rule_double[Pn - 1 + 3][Dn + 3];    //右上
//  Un[3] = rule_double[Pn + 3][Dn + 3];        //右下

    Un[0] = *(rule_single + (Pn - 1) * LISTLINGTH + (Dn - 1));//左上
    Un[1] = *(rule_single + (Pn) * LISTLINGTH + (Dn - 1));//左下
    Un[2] = *(rule_single + (Pn - 1) * LISTLINGTH + (Dn));//右上
    Un[3] = *(rule_single + (Pn) * LISTLINGTH + (Dn));//右下

    //求输出量化值
    U_p = (Un[0] * DF[0] * PF[0]
         + Un[1] * DF[0] * PF[1]
         + Un[2] * DF[1] * PF[0]
         + Un[3] * DF[1] * PF[1]) / 1000000;

    U = U_p * UFF_single;

    //printf("%f", U);

    return U;
}


#include "self_pid.h"

float Motor_speed=0;       //初始为0

// 舵机 PID 控制器结构体实例
_pid Servo_pid;
_pid Turn_Pid_L;
_pid Turn_Pid_R;
_pid Right_Motor_pid;
_pid Left_Motor_pid;

/**
 * @brief  舵机 PID 参数初始化
 * @note   设置目标值、误差和 PD 初始系数
 * @retval 无
 */
void PID_Servo_init(void)
{
    /* 初始化 PID 状态变量 */
    Servo_pid.target_val = 0.0; // 目标值
    Servo_pid.actual_val = 0.0; // 实际输出值（本次计算结果）
    Servo_pid.err = 0.0;        // 当前误差 e(k)
    Servo_pid.err_last = 0.0;   // 上上次误差 e(k-2)
    Servo_pid.err_next = 0.0;   // 上次误差 e(k-1)

    /* 设置初始 PID 控制器增益 */
    Servo_pid.Kp = 12.5; // 比例系数
    Servo_pid.Kd = 2.0;  // 微分系数
    // 注意：Ki 未在此处初始化，可能在别处设置或默认为 0
}

void Turn_Right_Init(void)
{
    Turn_Pid_R.actual_val=0;
    Turn_Pid_R.err=0;
    Turn_Pid_R.err_last=0;
    Turn_Pid_R.err_next=0;
    Turn_Pid_R.target_val=0;

    Turn_Pid_R.Kp=1.0;
    Turn_Pid_R.Kd=0;
}

void PID_Right_Motor_init(void)
{
   /* 初始化参数 */
    Right_Motor_pid.target_val=Motor_speed;
    Right_Motor_pid.actual_val=0.0;
    Right_Motor_pid.err = 0.0;
    Right_Motor_pid.err_last = 0.0;
    Right_Motor_pid.err_next = 0.0;

    Right_Motor_pid.Kp=510;
    Right_Motor_pid.Ki=17;
    Right_Motor_pid.Kd=0;

}

void PID_Left_Motor_init(void)
{
   /* 初始化参数 */
    Left_Motor_pid.target_val=Motor_speed;
    Left_Motor_pid.actual_val=0.0;
    Left_Motor_pid.err = 0.0;
    Left_Motor_pid.err_last = 0.0;
    Left_Motor_pid.err_next = 0.0;

        Left_Motor_pid.Kp=510;
        Left_Motor_pid.Ki=17;
        Left_Motor_pid.Kd=0;
}

//控制代码//
void set_pid_Right_Motor_target(float temp_val)
{
    Right_Motor_pid.target_val = temp_val;    // 设置当前的目标值
}

void set_pid_Left_Motor_target(float temp_val)
{
    Left_Motor_pid.target_val = temp_val;    // 设置当前的目标值
}


void Set_Turn_Target_L(float temp)
{
    Turn_Pid_L.target_val=temp;
}

void Set_Turn_Target_R(float temp)
{
    Turn_Pid_R.target_val=temp;
}

float PID_Right_Turn_realize(float error)
{
    Turn_Pid_R.actual_val=Turn_Pid_R.Kp*error + Turn_Pid_R.Kd*(error-Turn_Pid_R.err_next);

    /*传递误差*/
    Turn_Pid_R.err_next = error;

    /*返回当前实际值*/
    return Turn_Pid_R.actual_val;
}


/**
 * @brief  设置舵机目标值（期望值）
 * @param  temp_val : 目标值
 * @retval 无
 */
void set_pid_Servo_target(float temp_val)
{
    Servo_pid.target_val = temp_val; // 设置当前的目标值
}

/**
 * @brief  设置舵机比例、积分、微分系数
 * @param  p : 比例系数 Kp
 * @param  i : 积分系数 Ki
 * @param  d : 微分系数 Kd
 * @retval 无
 */
void set_Servo_p_i_d(float p, float i, float d)
{
    Servo_pid.Kp = p; // 设置比例系数 Kp
    Servo_pid.Ki = i; // 设置积分系数 Ki
    Servo_pid.Kd = d; // 设置微分系数 Kd
}

/**
 * @brief  舵机 PID 算法实现（PD 控制器，带积分死区与清零策略）
 * @param  actual_val : 系统的实际测量值（如舵机角度）
 * @retval 本次 PID 计算出的输出控制量（致动器的输入）
 */
float PID_Servo_realize(float actual_val)   //是否可以说p是变化，i是累计,d是趋势
{
    // im_p_d_actual_val: P 和 D 项的输出
    // im_i_actual_val: I 项的累积值（积分项）
    static float im_p_d_actual_val = 0, im_i_actual_val = 0;
    float err_old; // 暂存当前的误差 e(k) 作为下次的 e(k-1)

    /* 1. 计算当前误差 e(k) */
    err_old = Servo_pid.err;
    // 限制误差范围：[-1.5699, 1.5699]，防止误差溢出或超调
    Servo_pid.err = Limit_ab_float(Servo_pid.target_val - actual_val, -1.5699, 1.5699); // 接近 +/- pi/2

    /* 2. 计算比例 P 项 和 微分 D 项 */
    // D 项使用 (当前误差 e(k) - 上次误差 e(k-1))，为增量式微分
    im_p_d_actual_val = Servo_pid.Kp * Servo_pid.err + Servo_pid.Kd * (Servo_pid.err - Servo_pid.err_next);

    /* 3. 积分 I 项的特殊处理 (抗积分饱和与积分分离) */

    // 积分死区：只有当误差大于 0.7 或小于 -0.7 时才累加积分
    if (Servo_pid.err > 0.7 || Servo_pid.err < -0.7)
    {
        // 积分累加，并限制 I 项的绝对值在 [-0.1, 0.1] 之间（抗饱和）
        im_i_actual_val = Limit_ab_float(im_i_actual_val + Servo_pid.Ki * Servo_pid.err, -0.1, 0.1);
    }

    // 积分清零（积分分离策略）：当误差进入较小的范围 [-0.4, 0.4] 时，强制清零 I 项
    if ((Servo_pid.err < 0.4 && err_old >= -0.4) || (Servo_pid.err > -0.4 && err_old <= 0.4))
    {
        im_i_actual_val = 0;
    }

    /* 4. 计算最终输出（注意：实际输出中 I 项被注释掉了，这是一个 PD 控制器）*/
    // Servo_pid.actual_val = im_p_d_actual_val + im_i_actual_val; // 完整的 PID 输出
    Servo_pid.actual_val = im_p_d_actual_val; // 实际只输出了 P + D 项

    /* 5. 传递误差 (更新历史误差) */
    Servo_pid.err_last = Servo_pid.err_next; // e(k-2) = e(k-1)
    Servo_pid.err_next = Servo_pid.err;      // e(k-1) = e(k)

    /* 返回本次计算结果 */
    return Servo_pid.actual_val;
}

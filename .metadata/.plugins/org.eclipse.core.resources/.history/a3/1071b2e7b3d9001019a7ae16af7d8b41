#include "Self_Control.h"

// 全局/外部变量声明 (已修复乱码)
uint8_t Time_Out;
uint8_t Cargo_flag;

// 机械几何参数 (cm)
float W = 15.5, L = 20;             // W 表示轮距 (Wheel Track)，L 表示轴距 (Wheelbase)
float Decision_speed = 25;          // 决策速度，控制算法希望车辆达到的目标速度。

float Servo_angle;                  // 舵机 PWM 占空比输出值
float Servo_error = 0, LoseSpeed, AddSpeed; // 舵机 PID 输入误差及其他速度变量

// 函数声明 (定义在文件底部)
void Ackermann_steering(float angle);

// 舵机控制主函数
void Servo_Control(float error)
{
    // 静态变量用于维持脱轨状态和上次误差
    static uint8_t outside = 0;       // 脱轨内部状态锁
    static float Servo_Last = 0;      // 上次的 Servo_error (用于微分或模糊控制)
    float servo_1 = 0;                // 实际转向角度 (弧度制)

    if (Cargo_flag == 1) // 检查任务或载荷标志，判断是否允许控制
    {
        // 正常循迹模式: 外部没有脱轨信号 (Outside_Flag==0) 且 内部锁未激活 (outside==0)
        if (Outside_Flag == 0 && outside == 0)
        {
            // --- 坡道元素特殊处理 (Element_Ramp) ---
            if (Element_Ramp.Lock == Lock)
            {
                 // 1. 误差归一化：使用函数参数传入的 'error' (可能为特殊坡道误差)
                 Servo_error = Limit_ab_float(error / 93.0 * (float)(Realwind) / 69.0, -1.1695, 1.1695);
                 Servo_Last = Servo_error; // 更新上次误差（用于下次微分或模糊输入）

                 // 2. PID 计算 PWM 值：将 PID 输出缩放到 PWM 域 [S_PWM_Min, S_PWM_Max]
                 Servo_angle = Limit_ab_float(S_PWM_Mid * (1 + (Limit_ab_float(PID_Servo_realize(Servo_error), -12.1, 12.1)) / 100), S_PWM_Min, S_PWM_Max);

                 // 3. PWM 值转换为角度 (弧度制):
                 // 计算其在 +/- 30度 (PI/6) 范围内的偏移比例，S_PWM_H 应为新域的半行程
                 servo_1 = (float)(Servo_angle - S_PWM_Mid) / S_PWM_H * PI / 6.0;

                 // 4. 应用阿克曼差速补偿
                 Ackermann_steering(servo_1);

                 // 5. 输出 PWM 占空比
                 pwm_set_duty(SERVO_MOTOR_PWM, Servo_angle);
            }
            // --- 常规循迹模式 (非坡道) ---
            else
            {
                 // 1. 误差归一化：使用外部全局变量 'control_error' (常规循迹误差)
                 Servo_error = Limit_ab_float(control_error / 93.0 * (float)(Realwind) / 69.0, -1.1695, 1.1695);

                 // 2. PID 计算 PWM 值（同上）
                 Servo_angle = Limit_ab_float(S_PWM_Mid * (1 + (Limit_ab_float(PID_Servo_realize(Servo_error), -12.1, 12.1)) / 100), S_PWM_Min, S_PWM_Max);

                 // 3. PWM 值转换为角度 (同上)
                 servo_1 = (float)(Servo_angle - S_PWM_Mid) / S_PWM_H * PI / 6.0;

                 // 4. 应用阿克曼差速补偿
                 Ackermann_steering(servo_1);

                 // 5. 输出 PWM 占空比
                 pwm_set_duty(SERVO_MOTOR_PWM, Servo_angle);
            }
        }
        // --- 异常/脱轨模式 ---
        else
        {
           outside = 1; // 激活内部脱轨锁
           set_pid_Right_Motor_target(0); // 停止右电机
           set_pid_Left_Motor_target(0);  // 停止左电机

           // 虽然电机停止，但舵机仍需保持对赛道的响应：
           Servo_error = Limit_ab_float(control_error / 93.0 * (float)(Realwind) / 69.0, -1.1695, 1.1695);
           Servo_angle = Limit_ab_float(S_PWM_Mid * (1 + (Limit_ab_float(PID_Servo_realize(Servo_error), -12.1, 12.1)) / 100), S_PWM_Min, S_PWM_Max);

           pwm_set_duty(SERVO_MOTOR_PWM, Servo_angle);   // 舵机输出 PWM，但不进行阿克曼差速计算 (类似于急停时保持转向角)
        }
    }
}


/**
 * @brief  阿克曼转向差速补偿计算
 * @param  angle : 舵机实际转向角度 (弧度 rad)
 * @retval 无 (计算结果存入 Turn_Pid_R.actual_val)
 */
void Ackermann_steering(float angle)
{
    float R; // 瞬时旋转中心 (ICC) 半径 R

    if (angle == 0)
    {
        Turn_Pid_R.actual_val = 0; // 直线行驶，补偿值为 0
    }
    else
    {
        // 1. 计算转弯半径 R: R = L / tan(angle)
        R = L / Limit_ab_float(tan(angle), -100000, 100000); // 限制 tan(angle) 避免除零

        // 2. 计算速度补偿量 (Turn_Pid_R.actual_val)
        // 补偿量 = 目标速度 * 补偿增益 * 曲率因子(7.8/R)
        // 补偿量最终用于修正左右轮的目标速度，实现差速转向。
        Turn_Pid_R.actual_val = Decision_speed * Turn_Pid_R.Kp * Limit_ab_float(7.8 / R, -1.00, 1.00);
    }
}

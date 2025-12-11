
#include "Self_Servo.h"

void self_servo_init(void)
{
     pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0); 
}

void self_servo_process(void)
{
          pwm_set_duty(SERVO_MOTOR_PWM, S_PWM_Mid);

//        if(servo_motor_dir)
//        {
//            servo_motor_duty ++;
//            if(servo_motor_duty >= SERVO_MOTOR_R_MAX)
//            {
//                servo_motor_dir = 0x00;
//            }
//        }
//        else
//        {
//            servo_motor_duty --;
//            if(servo_motor_duty <= SERVO_MOTOR_L_MAX)
//            {
//                servo_motor_dir = 0x01;
//            }
//        }
        system_delay_ms(20);        
}
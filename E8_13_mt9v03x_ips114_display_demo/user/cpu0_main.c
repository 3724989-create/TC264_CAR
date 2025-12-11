
#include "Self_BSP.h"
#pragma section all "cpu0_dsram"

int core0_main(void)
{
    all_init();

	while (TRUE)
	{
       self_mtv90x_process();
	   Servo_Control((float)(Control_Mid-93));  //93表示图像中心
	}
}

#pragma section all restore

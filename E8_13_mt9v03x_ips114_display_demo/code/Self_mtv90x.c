
#include "Self_mtv90x.h"

void self_mtv90x_init(void)
{
    ips114_init();
    ips114_show_string(0, 0, "mt9v03x init.");
    while(1)
    {
        if(mt9v03x_init())
            ips114_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }

}

void self_mtv90x_process(void)
{
        if(mt9v03x_finish_flag)
        {
            ips114_displayimage03x((const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H);                             // 显示原始图像
//            ips114_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 240, 135, 64);       // 显示灰度图像
            mt9v03x_finish_flag = 0;
        }
}


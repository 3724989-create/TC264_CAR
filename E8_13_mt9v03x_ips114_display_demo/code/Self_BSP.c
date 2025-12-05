
#include "Self_BSP.h"

void all_init(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    self_motor_init();
    self_servo_init();
    self_mtv90x_init();
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
}
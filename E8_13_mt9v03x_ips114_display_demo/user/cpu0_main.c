
#include "Self_BSP.h"
#pragma section all "cpu0_dsram"

int core0_main(void)
{
    all_init();

	while (TRUE)
	{
       self_mtv90x_process();
	}
}

#pragma section all restore

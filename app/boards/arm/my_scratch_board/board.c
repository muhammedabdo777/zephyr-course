#include <zephyr/kernel.h>
#include <zephyr/init.h>

static int board_init(void)
{
	printk("Board Initialized\n");
	return 0;
}
SYS_INIT(board_init, PRE_KERNEL_1, 0);

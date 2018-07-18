#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/netfilter.h>

int init_module(void)
{
	printk(KERN_INFO "Starting module.\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Exiting....\n");
}


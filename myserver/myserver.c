/*  
 *   *  hello-1.c - The simplest kernel module.
 *    */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include "httpd.h"

int init_module(void)
{
		printk(KERN_INFO "Hello world 1.\n");

			/* 
			 * 	 * A non 0 return means init_module failed; module can't be loaded. 
			 * 	 	 */
		serve_forever("12913");
		return 0;
}

void cleanup_module(void)
{
		printk(KERN_INFO "Goodbye world 1.\n");
}

void route()
{
	ROUTE_START()

	ROUTE_GET("/")
	{
		printf("HTTP/1.1 200 OK\r\n\r\n");
		printf("Hello! You are using %s", request_header("User-Agent"));
	}

	ROUTE_END()
}

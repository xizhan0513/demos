#include <linux/init.h>
#include <linux/module.h>

extern char *module_a_str;
extern int module_a_function(void);

static int __init demo_init(void)
{
	module_a_function();
	printk("%s\n", module_a_str);
	printk("module init done!\n");
	return 0;
}

static void __exit demo_exit(void)
{
	printk("module exit done!\n");
	return ;
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

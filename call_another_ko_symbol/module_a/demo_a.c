#include <linux/init.h>
#include <linux/module.h>

char *module_a_str = "{Hello World} from module a!";
EXPORT_SYMBOL(module_a_str);

int module_a_function(void)
{
	printk("%s %d\n", __func__, __LINE__);

	return 0;
}
EXPORT_SYMBOL(module_a_function);

static int __init demo_init(void)
{
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

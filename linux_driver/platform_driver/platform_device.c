#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>

static struct resource demo_resource[] =
{
	[0] = {
		.start = 0x114000a0,
		.end   = 0x114000a0 + 0x4,
		.flags = IORESOURCE_MEM,
	},

	[1] = {
		.start = 0x139D0000,
		.end   = 0x139D0000 + 0x14,
		.flags = IORESOURCE_IRQ,
	}
};

static void demo_device_release(struct device *dev)
{
	printk("%s\n", __func__);

	return ;
}

static struct platform_device demo_device =
{
	.name          = "demo_use_devName",
	.id            = -1,
	.dev.release   = demo_device_release,
	.num_resources = ARRAY_SIZE(demo_resource),
	.resource      = demo_resource,
};

static int __init platform_device_init(void)
{
	printk("platform device init!\n");

	return platform_device_register(&demo_device);
}

static void __exit platform_device_exit(void)
{
	printk("platform device exit!\n");

	return platform_device_unregister(&demo_device);
}

module_init(platform_device_init);
module_exit(platform_device_exit);

MODULE_LICENSE("GPL");

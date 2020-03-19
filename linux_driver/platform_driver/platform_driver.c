#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

int demo_probe(struct platform_device *pdev)
{
	/* 1、申请设备号 */
	/* 2、cdev初始化注册 */
	/* 3、从pdev读出硬件资源 */
	/* 4、对硬件资源的初始化 */
	printk("%s\n", __func__);

	return 0;
}

int demo_remove(struct platform_device *pdev)
{
	printk("%s\n", __func__);

	return 0;
}

static const struct of_device_id demo_dt_ids[] =
{
	{ .compatible = "demo_use_dtsCompatible", },
	{ /* TODO */ }
};

static struct platform_driver demo_driver =
{
	.driver = {
		.name           = "demo_use_devName",
		.of_match_table = demo_dt_ids,
	},
	.probe  = demo_probe,
	.remove = demo_remove,
};

static int __init platform_driver_init(void)
{
	printk("platform driver init!\n");

	return platform_driver_register(&demo_driver);
}

static void __exit platform_driver_exit(void)
{
	printk("platform driver exit!\n");
	platform_driver_unregister(&demo_driver);

	return ;
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);

MODULE_LICENSE("GPL");

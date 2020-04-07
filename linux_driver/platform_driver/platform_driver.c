#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

int demo_probe(struct platform_device *pdev)
{
	/* 1、申请设备号 */
	/* 2、cdev初始化注册 */
	/* 3、从pdev读出硬件资源 */
	/* 4、对硬件资源的初始化 */

#if 0
	int irqno = 0;
	irqno = platform_get_irq(pdev, 0);
	if (irqno < 0) {
		printk("platform_get_irq failed!\n");
	} else {
		printk("irq number = %d\n", irqno);
	}
#endif

#if 0
	int irqno = 0;
	irqno = platform_get_irq_byname(pdev, "demo_int_name"); /* 如果设备树没有定义interrupt-names */
	if (irqno < 0) {                                        /* 则使用节点的路径代替，即"/demo_var" */
		printk("platform_get_irq_byname failed!\n");
	} else {
		printk("irq number = %d\n", irqno);
	}
#endif

#if 0
	struct resource *res;
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0); /* 如果是IORESOURCE_IRQ,则res->start是中断号 */
	if (res) {
		printk("start = %#x\n", res->start);
		printk("end   = %#x\n", res->end);
		printk("flags = %lu\n", res->flags);
		printk("name  = %s\n",  res->name);
	}
#endif

#if 0
	struct resource *res;
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "demo_reg_name");
	if (res) {
		printk("start = %#x\n", res->start);
		printk("end   = %#x\n", res->end);
		printk("flags = %lu\n", res->flags);
		printk("name  = %s\n",  res->name);
	}
#endif

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

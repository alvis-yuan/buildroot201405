#include <linux/err.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/mfd/core.h>

#include <irq.h>

#define JZ_REG_ADC_ENABLE       0x00
#define JZ_REG_ADC_CTRL         0x08
#define JZ_REG_ADC_STATUS       0x0c

#define JZ_REG_ADC_AUX_BASE		0x10
#define JZ_REG_ADC_CLKDIV		0x20

#define JZ_REG_ADC_STABLE		0x24
#define JZ_REG_ADC_REPEAT_TIME	0x28


#define CLKDIV			120
#define CLKDIV_US       2
#define CLKDIV_MS       100

#define STABLE_TIME		1
#define REPEAT_TIME		1

enum {
	JZ_ADC_IRQ_AUX = 0,
	JZ_ADC_IRQ_AUX1,
	JZ_ADC_IRQ_AUX2,
	JZ_ADC_IRQ_AUX3,
	JZ_ADC_IRQ_AUX4,
	JZ_ADC_IRQ_AUX5,
	JZ_ADC_IRQ_AUX6,
	JZ_ADC_IRQ_AUX7,
};

struct jz_adc {
	struct resource *mem;
	void __iomem *base;

	int irq;
	int irq_base;

	struct clk *clk;
	atomic_t clk_ref;

	spinlock_t lock;
};

static inline void jz_adc_irq_set_masked(struct jz_adc *adc, int irq,
		bool masked)
{
	unsigned long flags;
	uint8_t val;

	irq -= adc->irq_base;

	spin_lock_irqsave(&adc->lock, flags);

	val = readb(adc->base + JZ_REG_ADC_CTRL);
	if (masked) {
		val |= BIT(irq);
	}
	else {
		val &= ~BIT(irq);
	}
	writeb(val, adc->base + JZ_REG_ADC_CTRL);

	spin_unlock_irqrestore(&adc->lock, flags);
}

static void jz_adc_irq_mask(struct irq_data *data)
{
	struct jz_adc *adc = irq_data_get_irq_chip_data(data);
	jz_adc_irq_set_masked(adc, data->irq, true);
}

static void jz_adc_irq_unmask(struct irq_data *data)
{
	struct jz_adc *adc = irq_data_get_irq_chip_data(data);
	jz_adc_irq_set_masked(adc, data->irq, false);
}

static void jz_adc_irq_ack(struct irq_data *data)
{
	struct jz_adc *adc = irq_data_get_irq_chip_data(data);
	unsigned int irq = data->irq - adc->irq_base;
	writeb(BIT(irq), adc->base + JZ_REG_ADC_STATUS);
}

static struct irq_chip jz_adc_irq_chip = {
	.name = "jz-adc",
	.irq_mask = jz_adc_irq_mask,
	.irq_disable = jz_adc_irq_mask,
	.irq_unmask = jz_adc_irq_unmask,
	.irq_ack = jz_adc_irq_ack,
};

static void jz_adc_irq_demux(unsigned int irq, struct irq_desc *desc)
{
	struct jz_adc *adc = irq_desc_get_handler_data(desc);
	uint8_t status;
	unsigned int i;

	status = readb(adc->base + JZ_REG_ADC_STATUS);
	
 	for (i = 0; i < SADC_NR_IRQS; i++) {
		if (status & BIT(i)) {
			generic_handle_irq(adc->irq_base + i);
		}
	}
}

static inline void jz_adc_enable(struct jz_adc *adc)
{
	uint16_t val;

	if (atomic_inc_return(&adc->clk_ref) == 1) {
		val = readw(adc->base + JZ_REG_ADC_ENABLE);
		val &= ~BIT(15);
		val |= BIT(14) | BIT(9);
		writew(val, adc->base + JZ_REG_ADC_ENABLE);
		msleep(5);

		writew(STABLE_TIME, adc->base + JZ_REG_ADC_STABLE);
		writel(REPEAT_TIME, adc->base + JZ_REG_ADC_REPEAT_TIME);
	}
}

static inline void jz_adc_disable(struct jz_adc *adc)
{
	uint16_t val;

	if (atomic_dec_return(&adc->clk_ref) == 0) {
		val = readw(adc->base + JZ_REG_ADC_ENABLE);
		val |= BIT(15);
		writew(val, adc->base + JZ_REG_ADC_ENABLE);
	}
}

static inline void jz_adc_set_enabled(struct jz_adc *adc, int engine,
		bool enabled)
{
	unsigned long flags;
	uint16_t val;

	spin_lock_irqsave(&adc->lock, flags);

	val = readw(adc->base + JZ_REG_ADC_ENABLE);
	if (enabled) {
		val |= BIT(engine);
	}
	else {
		val &= ~BIT(engine);
	}
	writew(val, adc->base + JZ_REG_ADC_ENABLE);

	spin_unlock_irqrestore(&adc->lock, flags);
}

static int jz_adc_cell_enable(struct platform_device *pdev)
{
	struct jz_adc *adc = dev_get_drvdata(pdev->dev.parent);

	jz_adc_enable(adc);
	jz_adc_set_enabled(adc, pdev->id, true);

	return 0;
}

static int jz_adc_cell_disable(struct platform_device *pdev)
{
	struct jz_adc *adc = dev_get_drvdata(pdev->dev.parent);

	jz_adc_set_enabled(adc, pdev->id, false);
	jz_adc_disable(adc);

	return 0;
}

static void jz_adc_clk_div(struct jz_adc *adc, const unsigned char clkdiv,
		const unsigned char clkdiv_us, const unsigned short clkdiv_ms)
{
	unsigned int val;

	val = clkdiv | (clkdiv_us << 8) | (clkdiv_ms << 16);
	writel(val, adc->base + JZ_REG_ADC_CLKDIV);
}

static struct resource jz_aux_resources[] = {
	{
		.start = JZ_ADC_IRQ_AUX,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start	= JZ_REG_ADC_AUX_BASE,
		.end	= JZ_REG_ADC_AUX_BASE + 1,
		.flags	= IORESOURCE_MEM,
	},
};	

static struct resource jz_aux_resources1[] = {
	{
		.start = JZ_ADC_IRQ_AUX1,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start	= JZ_REG_ADC_AUX_BASE + 2,
		.end	= JZ_REG_ADC_AUX_BASE + 3,
		.flags	= IORESOURCE_MEM,
	},
};	

static struct resource jz_aux_resources2[] = {
	{
		.start = JZ_ADC_IRQ_AUX2,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start	= JZ_REG_ADC_AUX_BASE + 4,
		.end	= JZ_REG_ADC_AUX_BASE + 5,
		.flags	= IORESOURCE_MEM,
	},
};	

static struct resource jz_aux_resources3[] = {
	{
		.start = JZ_ADC_IRQ_AUX3,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start	= JZ_REG_ADC_AUX_BASE + 6,
		.end	= JZ_REG_ADC_AUX_BASE + 7,
		.flags	= IORESOURCE_MEM,
	},
};	

static struct resource jz_aux_resources4[] = {
	{
		.start = JZ_ADC_IRQ_AUX4,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start	= JZ_REG_ADC_AUX_BASE + 8,
		.end	= JZ_REG_ADC_AUX_BASE + 9,
		.flags	= IORESOURCE_MEM,
	},
};	

static struct resource jz_aux_resources5[] = {
	{
		.start = JZ_ADC_IRQ_AUX5,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start	= JZ_REG_ADC_AUX_BASE + 10,
		.end	= JZ_REG_ADC_AUX_BASE + 11,
		.flags	= IORESOURCE_MEM,
	},
};	

static struct resource jz_aux_resources6[] = {
	{
		.start = JZ_ADC_IRQ_AUX6,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start	= JZ_REG_ADC_AUX_BASE + 12,
		.end	= JZ_REG_ADC_AUX_BASE + 13,
		.flags	= IORESOURCE_MEM,
	},
};	

static struct resource jz_aux_resources7[] = {
	{
		.start = JZ_ADC_IRQ_AUX7,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start	= JZ_REG_ADC_AUX_BASE + 14,
		.end	= JZ_REG_ADC_AUX_BASE + 15,
		.flags	= IORESOURCE_MEM,
	},
};	


static struct mfd_cell jz_adc_cells[] = {
	{
		.id = 0,
		.name = "jz-aux",
		.num_resources = ARRAY_SIZE(jz_aux_resources),
		.resources = jz_aux_resources,

		.enable	= jz_adc_cell_enable,
		.disable = jz_adc_cell_disable,
	},
	{
		.id = 1,
		.name = "jz-aux",
		.num_resources = ARRAY_SIZE(jz_aux_resources1),
		.resources = jz_aux_resources1,

		.enable	= jz_adc_cell_enable,
		.disable = jz_adc_cell_disable,
	},	
	{
		.id = 2,
		.name = "jz-aux",
		.num_resources = ARRAY_SIZE(jz_aux_resources2),
		.resources = jz_aux_resources2,

		.enable	= jz_adc_cell_enable,
		.disable = jz_adc_cell_disable,
	},
	{
		.id = 3,
		.name = "jz-aux",
		.num_resources = ARRAY_SIZE(jz_aux_resources3),
		.resources = jz_aux_resources3,

		.enable	= jz_adc_cell_enable,
		.disable = jz_adc_cell_disable,
	},
	{
		.id = 4,
		.name = "jz-aux",
		.num_resources = ARRAY_SIZE(jz_aux_resources4),
		.resources = jz_aux_resources4,

		.enable	= jz_adc_cell_enable,
		.disable = jz_adc_cell_disable,
	},
	{
		.id = 5,
		.name = "jz-aux",
		.num_resources = ARRAY_SIZE(jz_aux_resources5),
		.resources = jz_aux_resources5,

		.enable	= jz_adc_cell_enable,
		.disable = jz_adc_cell_disable,
	},
	{
		.id = 6,
		.name = "jz-aux",
		.num_resources = ARRAY_SIZE(jz_aux_resources6),
		.resources = jz_aux_resources6,

		.enable	= jz_adc_cell_enable,
		.disable = jz_adc_cell_disable,
	},
	{
		.id = 7,
		.name = "jz-aux",
		.num_resources = ARRAY_SIZE(jz_aux_resources7),
		.resources = jz_aux_resources7,

		.enable	= jz_adc_cell_enable,
		.disable = jz_adc_cell_disable,
	},

};

static struct jz_adc *test_adc;

int test_jz_adc_enable(unsigned int id)
{
	if(test_adc == NULL || id > SADC_NR_IRQS)
		return -1;

	jz_adc_enable(test_adc);
	jz_adc_set_enabled(test_adc, id, true);

	return 0;
}
EXPORT_SYMBOL(test_jz_adc_enable);

int test_jz_adc_disable(unsigned int id)
{
	if(test_adc == NULL || id > SADC_NR_IRQS)
		return -1;

	jz_adc_set_enabled(test_adc, id, false);
	jz_adc_disable(test_adc);

	return 0;
}
EXPORT_SYMBOL(test_jz_adc_disable);

int test_jz_adc_read(unsigned int id)
{
	uint8_t status;
	int val;

	if(test_adc == NULL || id > SADC_NR_IRQS)
		return -1;

	do{
		status = readb(test_adc->base + JZ_REG_ADC_STATUS);
	} while(!(status & BIT(id)));

	val = readl(test_adc->base + JZ_REG_ADC_AUX_BASE + (id / 2) * 4);
	val = val >> ((id % 2) * 16);
	val = val & 0xfff;

	writeb(BIT(id), test_adc->base + JZ_REG_ADC_STATUS);

	return val;
}
EXPORT_SYMBOL(test_jz_adc_read);


static int jz_adc_probe(struct platform_device *pdev)
{
	int ret;
	struct jz_adc *adc;
	struct resource *mem_base;
	int irq;
	unsigned char clkdiv, clkdiv_us;
	unsigned short clkdiv_ms;

	adc = kmalloc(sizeof(*adc), GFP_KERNEL);
	if (!adc) {
		dev_err(&pdev->dev, "Failed to allocate driver structre\n");
		return -ENOMEM;
	}

	adc->irq = platform_get_irq(pdev, 0);
	if (adc->irq < 0) {
		ret = adc->irq;
		dev_err(&pdev->dev, "Failed to get platform irq: %d\n", ret);
		goto err_free;
	}

	adc->irq_base = platform_get_irq(pdev, 1);
	if (adc->irq_base < 0) {
		ret = adc->irq_base;
		dev_err(&pdev->dev, "Failed to get irq base: %d\n", ret);
		goto err_free;
	}

	mem_base = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem_base) {
		ret = -ENOENT;
		dev_err(&pdev->dev, "Failed to get platform mmio resource");
		goto err_free;
	}

	adc->mem = request_mem_region(mem_base->start, JZ_REG_ADC_STATUS,
			pdev->name);
	if (!adc->mem) {
		ret = -EBUSY;
		dev_err(&pdev->dev, "Failed to request mmio memory region\n");
		goto err_free;
	}

	adc->base = ioremap_nocache(adc->mem->start, resource_size(adc->mem));
	if (!adc->base) {
		ret = -EBUSY;
		dev_err(&pdev->dev, "Failed to ioremap mmio memory\n");
		goto err_release_mem_region;
	}

	adc->clk = clk_get(&pdev->dev, "sadc");
	if (IS_ERR(adc->clk)) {
		ret = PTR_ERR(adc->clk);
		dev_err(&pdev->dev, "Failed to get clock: %d\n", ret);
		goto err_iounmap;
	}

	spin_lock_init(&adc->lock);
	atomic_set(&adc->clk_ref, 0);

	platform_set_drvdata(pdev, adc);

	for (irq = adc->irq_base; irq < adc->irq_base + SADC_NR_IRQS; ++irq) {
		irq_set_chip_data(irq, adc);
		irq_set_chip_and_handler(irq, &jz_adc_irq_chip,
				handle_level_irq);
	}

	irq_set_handler_data(adc->irq, adc);
	irq_set_chained_handler(adc->irq, jz_adc_irq_demux);

	clk_enable(adc->clk);

	writew(0x8000, adc->base + JZ_REG_ADC_ENABLE);
	writew(0xffff, adc->base + JZ_REG_ADC_CTRL);

	clkdiv = CLKDIV - 1;
	clkdiv_us = CLKDIV_US - 1;
	clkdiv_ms = CLKDIV_MS - 1;

	jz_adc_clk_div(adc, clkdiv, clkdiv_us, clkdiv_ms);
	
	ret = mfd_add_devices(&pdev->dev, 0, jz_adc_cells,
			SADC_NR_IRQS, mem_base, adc->irq_base,NULL);
	if (ret < 0) {
		goto err_clk_put;
	}

	printk("jz SADC driver registeres over!\n");

	test_adc = adc;

	return 0;

err_clk_put:
	clk_put(adc->clk);
err_iounmap:
	platform_set_drvdata(pdev, NULL);
	iounmap(adc->base);
err_release_mem_region:
	release_mem_region(adc->mem->start, resource_size(adc->mem));
err_free:
	kfree(adc);

	return ret;
}

static int jz_adc_remove(struct platform_device *pdev)
{
	struct jz_adc *adc = platform_get_drvdata(pdev);

	clk_disable(adc->clk);
	mfd_remove_devices(&pdev->dev);

	irq_set_handler_data(adc->irq, NULL);
	irq_set_chained_handler(adc->irq, NULL);

	iounmap(adc->base);
	release_mem_region(adc->mem->start, resource_size(adc->mem));

	clk_put(adc->clk);

	platform_set_drvdata(pdev, NULL);

	kfree(adc);

	test_adc = NULL;
	return 0;
}

struct platform_driver jz_adc_driver = {
	.probe	= jz_adc_probe,
	.remove	= jz_adc_remove,
	.driver = {
		.name	= "jz-adc",
		.owner	= THIS_MODULE,
	},
};

static int __init jz_adc_init(void)
{
	return platform_driver_register(&jz_adc_driver);
}
module_init(jz_adc_init);

static void __exit jz_adc_exit(void)
{
	platform_driver_unregister(&jz_adc_driver);
}
module_exit(jz_adc_exit);

MODULE_DESCRIPTION("JZ SOC ADC driver");
MODULE_AUTHOR("xinshuan	<shuan.xin@ingenic.com>");
MODULE_LICENSE("GPL");
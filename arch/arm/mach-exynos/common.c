/*
 * Copyright (c) 2010-2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Common Codes for EXYNOS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/irqchip.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <clocksource/samsung_pwm.h>
#include <linux/sched.h>
#include <linux/serial_core.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/pm_domain.h>
#include <linux/export.h>
#include <linux/irqdomain.h>
#include <linux/of_address.h>
#include <linux/irqchip/arm-gic.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/platform_device.h>

#include <asm/proc-fns.h>
#include <asm/exception.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/cacheflush.h>

#include <plat/cpu.h>
#include <plat/pm.h>
#include <plat/regs-serial.h>

#include "common.h"
#include "regs-pmu.h"

#define L2_AUX_VAL 0x7C470001
#define L2_AUX_MASK 0xC200ffff

static struct platform_device exynos_cpuidle = {
	.name		= "exynos_cpuidle",
	.id		= -1,
};

void __init exynos_cpuidle_init(void)
{
	platform_device_register(&exynos_cpuidle);
}

void __init exynos_cpufreq_init(void)
{
	platform_device_register_simple("exynos-cpufreq", -1, NULL, 0);
}

struct bus_type exynos_subsys = {
	.name		= "exynos-core",
	.dev_name	= "exynos-core",
};

static struct device exynos4_dev = {
	.bus	= &exynos_subsys,
};

static int __init exynos_core_init(void)
{
	return subsys_system_register(&exynos_subsys, NULL);
}
core_initcall(exynos_core_init);

static int __init exynos4_l2x0_cache_init(void)
{
	int ret;

	ret = l2x0_of_init(L2_AUX_VAL, L2_AUX_MASK);
	if (ret)
		return ret;

	l2x0_regs_phys = virt_to_phys(&l2x0_saved_regs);
	clean_dcache_area(&l2x0_regs_phys, sizeof(unsigned long));
	return 0;
}
early_initcall(exynos4_l2x0_cache_init);

static int __init exynos_init(void)
{
	printk(KERN_INFO "EXYNOS: Initializing architecture\n");

	return device_register(&exynos4_dev);
}

/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Common Header for EXYNOS machines
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MACH_EXYNOS_COMMON_H
#define __ARCH_ARM_MACH_EXYNOS_COMMON_H

#include <linux/reboot.h>
#include <linux/of.h>

#define soc_is_exynos4() (soc_is_exynos4210() || soc_is_exynos4212() || \
              soc_is_exynos4412())
#define soc_is_exynos5() (soc_is_exynos5250() || soc_is_exynos5410() || \
              soc_is_exynos5420())

extern u32 cp15_save_diag;
extern u32 cp15_save_power;
extern unsigned long l2x0_regs_phys;

extern void __iomem *sysram_ns_base_addr;
extern void __iomem *sysram_base_addr;
extern void __iomem *pmu_base_addr;
void exynos_sysram_init(void);

void exynos_firmware_init(void);

extern u32 exynos_get_eint_wake_mask(void);

#ifdef CONFIG_PM_SLEEP
extern void __init exynos_pm_init(void);
#else
static inline void exynos_pm_init(void) {}
#endif

extern void exynos_cpu_resume(void);
extern void exynos_cpu_resume_ns(void);

extern struct smp_operations exynos_smp_ops;

extern void exynos_cpu_die(unsigned int cpu);

/* PMU(Power Management Unit) support */

#define PMU_TABLE_END	(-1U)

enum sys_powerdown {
    SYS_AFTR,
    SYS_LPA,
    SYS_SLEEP,
    NUM_SYS_POWERDOWN,
};

struct exynos_pmu_conf {
    unsigned int offset;
    unsigned int val[NUM_SYS_POWERDOWN];
};

extern void exynos_sys_powerdown_conf(enum sys_powerdown mode);
extern void exynos_cpu_power_down(int cpu);
extern void exynos_cpu_power_up(int cpu);
extern int  exynos_cpu_power_state(int cpu);
extern void exynos_cluster_power_down(int cluster);
extern void exynos_cluster_power_up(int cluster);
extern int  exynos_cluster_power_state(int cluster);
extern void exynos_enter_aftr(void);

extern void s5p_init_cpu(void __iomem *cpuid_addr);
extern unsigned int samsung_rev(void);

static inline void pmu_raw_writel(u32 val, u32 offset)
{
    __raw_writel(val, pmu_base_addr + offset);
}

static inline u32 pmu_raw_readl(u32 offset)
{
    return __raw_readl(pmu_base_addr + offset);
}

#endif /* __ARCH_ARM_MACH_EXYNOS_COMMON_H */

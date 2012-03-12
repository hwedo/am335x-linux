/*
 * AM33Xx Power domains framework
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>

#include "powerdomain.h"
#include "prcm-common.h"
#include "prm33xx.h"

static struct powerdomain gfx_33xx_pwrdm = {
	.name			= "gfx_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= AM33XX_PRM_GFX_MOD,
	.pwrstctrl_offs		= AM33XX_PM_GFX_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= AM33XX_PM_GFX_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_RET_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF_RET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.banks			= 1,
	.logicretstate_mask	= (0x1 << 2),
	.mem_on_mask		= {
		[0]		= (0x3 << 17),		/* gfx_mem */
	},
	.mem_ret_mask		= {
		[0]		= (0x1 << 6),		/* gfx_mem */
	},
	.mem_pwrst_mask		= {
		[0]		= (0x3 << 4),		/* gfx_mem */
	},
	.mem_retst_mask		= {
		[0]		= (0x1 << 6),		/* gfx_mem */
	},
	.pwrsts_mem_ret		= {
		[0] 		= PWRSTS_OFF_RET,	/* gfx_mem */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* gfx_mem */
	},
};

static struct powerdomain rtc_33xx_pwrdm = {
	.name			= "rtc_pwrdm",
	.voltdm			= { .name = "rtc" },
	.prcm_offs		= AM33XX_PRM_RTC_MOD,
	.pwrstctrl_offs		= AM33XX_PM_RTC_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= AM33XX_PM_RTC_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_ON,
	.logicretstate_mask	= (0x1 << 2),
};

static struct powerdomain wkup_33xx_pwrdm = {
	.name			= "wkup_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= AM33XX_PRM_WKUP_MOD,
	.pwrstctrl_offs		= AM33XX_PM_WKUP_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= AM33XX_PM_WKUP_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_ON,
	.logicretstate_mask	= (0x1 << 3),
};

static struct powerdomain per_33xx_pwrdm = {
	.name			= "per_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= AM33XX_PRM_PER_MOD,
	.pwrstctrl_offs		= AM33XX_PM_PER_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= AM33XX_PM_PER_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_RET_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF_RET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.banks			= 3,
	.logicretstate_mask	= (0x1 << 3),
	.mem_on_mask		= {
		[0]		= (0x3 << 5),		/* pruss_mem */
		[1]		= (0x3 << 25),		/* per_mem */
		[2]		= (0x3 << 30),		/* ram_mem */
	},
	.mem_ret_mask		= {
		[0]		= (0x1 << 7),		/* pruss_mem */
		[1]		= (0x1 << 29),		/* per_mem */
		[2]		= (0x1 << 27),		/* ram_mem */
	},
	.mem_pwrst_mask		= {
		[0]		= (0x3 << 23),		/* pruss_mem */
		[1]		= (0x3 << 17),		/* per_mem */
		[2]		= (0x3 << 21),		/* ram_mem */
	},
	.mem_retst_mask		= {
		[0]		= (0x1 << 7),		/* pruss_mem */
		[1]		= (0x1 << 29),		/* per_mem */
		[2]		= (0x1 << 27),		/* ram_mem */
	},
	.pwrsts_mem_ret		= {
		[0]		= PWRSTS_OFF_RET,	/* pruss_mem */
		[1]		= PWRSTS_OFF_RET,	/* per_mem */
		[2]		= PWRSTS_OFF_RET,	/* ram_mem */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* pruss_mem */
		[1]		= PWRSTS_ON,		/* per_mem */
		[2]		= PWRSTS_ON,		/* ram_mem */
	},
};

static struct powerdomain mpu_33xx_pwrdm = {
	.name			= "mpu_pwrdm",
	.voltdm			= { .name = "mpu" },
	.prcm_offs		= AM33XX_PRM_MPU_MOD,
	.pwrstctrl_offs		= AM33XX_PM_MPU_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= AM33XX_PM_MPU_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_RET_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF_RET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.banks			= 3,
	.logicretstate_mask	= (0x1 << 2),
	.mem_on_mask		= {
		[0]		= (0x3 << 18),		/* mpu_l1 */
		[1]		= (0x3 << 20),		/* mpu_l2 */
		[2]		= (0x3 << 16),		/* mpu_ram */
	},
	.mem_ret_mask		= {
		[0]		= (0x1 << 22),		/* mpu_l1 */
		[1]		= (0x1 << 23),		/* mpu_l2 */
		[2]		= (0x1 << 24),		/* mpu_ram */
	},
	.mem_pwrst_mask		= {
		[0]		= (0x3 << 6),		/* mpu_l1 */
		[1]		= (0x3 << 8),		/* mpu_l2 */
		[2]		= (0x3 << 4),		/* mpu_ram */
	},
	.mem_retst_mask		= {
		[0]		= (0x1 << 22),		/* mpu_l1 */
		[1]		= (0x1 << 23),		/* mpu_l2 */
		[2]		= (0x1 << 24),		/* mpu_ram */
	},
	.pwrsts_mem_ret		= {
		[0]		= PWRSTS_OFF_RET,	/* mpu_l1 */
		[1]		= PWRSTS_OFF_RET,	/* mpu_l2 */
		[2]		= PWRSTS_OFF_RET,	/* mpu_ram */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* mpu_l1 */
		[1]		= PWRSTS_ON,		/* mpu_l2 */
		[2]		= PWRSTS_ON,		/* mpu_ram */
	},
};

static struct powerdomain cefuse_33xx_pwrdm = {
	.name		= "cefuse_pwrdm",
	.voltdm		= { .name = "core" },
	.prcm_offs	= AM33XX_PRM_CEFUSE_MOD,
	.pwrstctrl_offs	= AM33XX_PM_CEFUSE_PWRSTCTRL_OFFSET,
	.pwrstst_offs	= AM33XX_PM_CEFUSE_PWRSTST_OFFSET,
	.pwrsts		= PWRSTS_OFF_ON,
};

static struct powerdomain *powerdomains_am33xx[] __initdata = {
	&gfx_33xx_pwrdm,
	&rtc_33xx_pwrdm,
	&wkup_33xx_pwrdm,
	&per_33xx_pwrdm,
	&mpu_33xx_pwrdm,
	&cefuse_33xx_pwrdm,
	NULL,
};

void __init am33xx_powerdomains_init(void)
{
	pwrdm_register_platform_funcs(&am33xx_pwrdm_operations);
	pwrdm_register_pwrdms(powerdomains_am33xx);
	pwrdm_complete_init();
}

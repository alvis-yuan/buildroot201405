#ifndef __A3L28E40BGD_AHJA_H
#define	__A3L28E40BGD_AHJA_H

/*
 * CL:3,50M ~ 200M
 * CL:4,200M ~ 266M
 * CL:5,266M ~ 400M
 * CL:6,
 * CL:7,400M ~ 533M
 *
 * */
#ifndef CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ
#define CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ CONFIG_SYS_MEM_FREQ
#endif

#if (CONFIG_DDR_SEL_PLL == MPLL)
#define CONFIG_SYS_PLL_FREQ CONFIG_SYS_MPLL_FREQ
#else
#define CONFIG_SYS_PLL_FREQ CONFIG_SYS_APLL_FREQ
#endif

#define CONFIG_DDR_DATA_RATE (CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ * 2)

#if((CONFIG_SYS_PLL_FREQ % CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ) ||\
	(CONFIG_SYS_PLL_FREQ / CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ < 0) ||\
	(CONFIG_SYS_PLL_FREQ / CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ > 15))
#error DDR memoryclock division ratio should be an integer between 1 and 16, check CONFIG_SYS_MPLL_FREQ and CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ
#endif

#if ((CONFIG_DDR_DATA_RATE > 100000000) &&\
		(CONFIG_DDR_DATA_RATE <= 400000000))
#define CONFIG_DDR_CL	3
#elif((CONFIG_DDR_DATA_RATE > 400000000) &&\
		(CONFIG_DDR_DATA_RATE <= 533000000))
#define CONFIG_DDR_CL	4
#elif((CONFIG_DDR_DATA_RATE > 533000000) &&\
		(CONFIG_DDR_DATA_RATE <= 800000000))
#define CONFIG_DDR_CL	5
#elif((CONFIG_DDR_DATA_RATE > 800000000) &&\
		(CONFIG_DDR_DATA_RATE <= 1066000000))
#define CONFIG_DDR_CL	7
#else
#define CONFIG_DDR_CL	-1
#endif

#define CONFIG_DDR_AL	0

#if(-1 == CONFIG_DDR_CL)
#error CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ don't support, check data_rate range
#endif

static inline void LVDDR2_A3L28E40BGD_AHJA_init(void *data)
{
	struct ddr_chip_info *c = (struct ddr_chip_info *)data;
	unsigned int RL = CONFIG_DDR_CL + CONFIG_DDR_AL;

	c->DDR_ROW = 13;
	c->DDR_ROW1 = 13;
	c->DDR_COL = 10;
	c->DDR_COL1 = 10;
	c->DDR_BANK8 = 2;
	c->DDR_CL = CONFIG_DDR_CL;


	c->DDR_tRAS = DDR__ns(45);
	c->DDR_tRP = DDR__ns(20);
	c->DDR_tRCD = DDR__ns(20);
	c->DDR_tRC = (c->DDR_tRAS + c->DDR_tRP);
	c->DDR_tRRD = DDR__ns(10);
	c->DDR_tWR = DDR__ns(15);
	c->DDR_tWTR = DDR__ps(7500);

	c->DDR_tRFC = DDR__ns(75);

	c->DDR_tXP = DDR__tck(2);
	c->DDR_tMRD = DDR__tck(2);

	c->DDR_BL = 8;
	c->DDR_RL = DDR__tck(RL);
	c->DDR_WL = DDR__tck(RL - 1);

	c->DDR_tCKE = DDR__tck(3);
	c->DDR_tXSR = DDR__tck(200);
	c->DDR_tREFI = DDR__ns(4880);

#ifdef CONFIG_LVDDR_INNOPHY
	c->DDR_tRTP = DDR__ps(7500);
	c->DDR_tCCD = DDR__tck(2);
	c->DDR_tRTW = (((c->DDR_BL > 4) ? 6 : 4) + 1);
	c->DDR_tFAW = DDR__ns(45);

	c->DDR_tXARD = DDR__tck(2);
	c->DDR_tXARDS = DDR__tck(7);
	c->DDR_tXSNR = (c->DDR_tRFC + DDR__ns(10));
	c->DDR_tXSRD = DDR__tck(200);

	c->DDR_tCKESR = DDR__tck(3);
	c->DDR_tCKSRE = DDR__ns(10000);

	c->DDR_CLK_DIV = 1;
#endif
}

#define LVDDR2_A3L28E40BGD_AHJA {					\
	.name 	= "A3L28E40BGD-AHJA",					\
	.id	= DDR_CHIP_ID(VENDOR_ZENTEL, TYPE_DDR2, MEM_16M),	\
	.type	= DDR2,						\
	.freq	= CONFIG_LVDDR2_A3L28E40BGD_AHJA_MEM_FREQ,			\
	.size	= 16,						\
	.init	= LVDDR2_A3L28E40BGD_AHJA_init,				\
}

#endif /* __MDDR_CONFIG_H */



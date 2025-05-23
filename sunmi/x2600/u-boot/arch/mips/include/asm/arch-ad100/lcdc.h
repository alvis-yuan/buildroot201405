/*
 * Ingenic ad100 LCDC reg definition
 *
 * Copyright (C) 2016 Ingenic Semiconductor Inc.
 *
 * Author:clwang<chunlei.wang@ingenic.com>
 *
 * This program is free software, you can redistribute it and/or modify it
 *
 * under the terms of the GNU General Public License version 2 as published by
 *
 * the Free Software Foundation.
 */

#ifndef _DPU_REG_H_
#define _DPU_REG_H_

#define GENMASK(hi, lo)   (((1ULL << ((hi) - (lo) + 1)) - 1) << (lo))
#define         DPU_BASE	 	0xb3050000

/*-------------------------------------------------------------------------------
 *			      DPU Register Offset
 * -----------------------------------------------------------------------------*/


/* RW	32	0x0000_0000	frame descriptor's address*/
#define DC_FRM_CFG_ADDR		        (0x0000)
/* -W	32	0x0000_0000	frame descriptor's control*/
#define DC_FRM_CFG_CTRL		        (0x0004)
/* RW	32	0x0000_0000	RDMA descriptor's address*/
#define DC_RDMA_CHAIN_ADDR              (0x1000)
/* -W	32	0x0000_0000	RDMA descriptor's control*/
#define DC_RDMA_CHAIN_CTRL              (0x1004)
/* RW	32	0x0000_0000	DC control*/
#define DC_CTRL				(0x2000)
/* RW	32	0x0000_0000	DC status*/
#define DC_ST				(0x2004)
/* -W	32	0x0000_0000	DC clear status*/
#define DC_CLR_ST			(0x2008)
/* RW	32	0x0000_0000	DC interrupt mask*/
#define DC_INTC				(0x200c)
/* R-	32	0x0000_0000	For debug*/
#define DC_INT_FLAG			(0x2010)
/* RW	32	0x0000_0000	Common configure register*/
#define DC_COM_CONFIG		        (0x2014)

/* RW	32	0x0000-0006	RDMA Priority level soft config*/
#define DC_PCFG_RD_CTRL		        (0x2018)
/* RW	32	0x0000-0006	writeback Priority level soft config*/
#define DC_PCFG_WR_CTRL		        (0x201c)
/* RW	32	0x0066-4422	OFIFO Priority level threshold config*/
#define DC_OFIFO_PCFG		        (0x2020)
/* RW	32	0x0018-1008	WDMA Priority level threshold config*/
#define DC_WDMA_PCFG		        (0x2024)
/* RW	32	0x0000-0006	CMP_W Priority level soft config*/
#define DC_CMPW_PCFG_CTRL		(0x2028)
/* RW	32	0x0000-0006	CMPW PCFG0 soft config*/
#define DC_CMPW_PCFG0			(0x202c)
/* RW	32	0x0000-0006	CMPW PCFG1 soft config*/
#define DC_CMPW_PCFG1			(0x2030)
/* RW	32	0x0000-0006	CMPW PCFG2 soft config*/
#define DC_CMPW_PCFG2			(0x2034)

/* R-	32	0x0000_0000	For debug*/
#define DC_FRM_DES			(0x2100)
/* R-	32	0x0000_0000	For debug*/
#define DC_LAY0_DES			(0x2104)
/* R-	32	0x0000_0000	For debug*/
#define DC_LAY1_DES			(0x2108)
/* R-	32	0x0000_0000	For debug*/
#define DC_LAY2_DES			(0x210c)
/* R-	32	0x0000_0000	For debug*/
#define DC_LAY3_DES			(0x2110)
/* R-	32	0x0000_0000	For debug*/
#define DC_RDMA_DES			(0x2114)
/* R-	32	0x0000_0000	frame descriptor's current site*/
#define DC_FRM_CHAIN_SITE		(0x2200)
/* R-	32	0x0000_0000	rdma descriptor's current site*/
#define DC_RDMA_CHAIN_SITE		(0x2204)
/* R-	32	0x0000_0000	layer0's current site*/
#define DC_LAY0_SITE			(0x3100)
/* R-	32	0x0000_0000	layer1's current site*/
#define DC_LAY1_SITE			(0x3104)
/* R-	32	0x0000_0000	layer2's current site*/
#define DC_LAY2_SITE			(0x3108)
/* R-	32	0x0000_0000	layer3's current site*/
#define DC_LAY3_SITE			(0x310c)
/* R-	32	0x0000_0000	rdma's current site*/
#define DC_RDMA_SITE			(0x3110)
/* R-	32	0x0000_0000	wdma's current site*/
#define DC_WDMA_SITE			(0x221c)
/* RW	32	0x0000_0000	TLB Global Control*/
#define DC_TLB_GLBC			(0x3000)
/* RW	32	0x0000_0000	TLB table entry address*/
#define DC_TLB_TLBA			(0x3010)
/* RW	32	0x0000_0000	TLB trigger control*/
#define DC_TLB_TLBC			(0x3020)
/* R-	32	0x0000_0000	VPN probe 0*/
#define DC_TLB0_VPN			(0x3030)
/* R-	32	0x0000_0000	VPN probe 1*/
#define DC_TLB1_VPN			(0x3034)
/* R-	32	0x0000_0000	VPN probe 2*/
#define DC_TLB2_VPN			(0x3038)
/* R-	32	0x0000_0000	VPN probe 3*/
#define DC_TLB3_VPN			(0x303c)
/* RW	32	0x0000_0000	TLB table entry verification*/
#define DC_TLB_TLBV			(0x3040)
/* R-	32	0x0000_0000	TLB status*/
#define DC_TLB_STAT			(0x3050)
/* RW   32      0x066204a8	CscMultYRv*/
#define DC_CSC_MULT_YRV			(0x3200)
/* RW   32      0x03410190      CscMultGuGv*/
#define DC_CSC_MULT_GUGV		(0x3204)
/* RW   32      0x00000812      CscMultBu*/
#define DC_CSC_MULT_BU			(0x3208)
/* RW   32      0x00800000      CscSubYUV*/
#define DC_CSC_SUB_YUV			(0x320c)
/* RW	32	0x0000_0000	display common configure*/
#define DC_DISP_COM			(0x8000)
/* RW	32	0x0000_0000	TFT HSYNC*/
#define DC_TFT_HSYNC		        (0x9000)
/* RW	32	0x0000_0000	TFT VSYNC*/
#define DC_TFT_VSYNC		        (0x9004)
/* RW	32	0x0000_0000	TFT HDE*/
#define DC_TFT_HDE			(0x9008)
/* RW	32	0x0000_0000	TFT VDE*/
#define DC_TFT_VDE			(0x900C)
/* RW	32	0x0000_0000	TFT configure*/
#define DC_TFT_CFG			(0x9010)
/* RW	32	0x0000_0000	TFT status*/
#define DC_TFT_ST			(0x9014)
/* RW	32	0x0000_0000	SLCD configure*/
#define DC_SLCD_CFG			(0xA000)
/* RW	32	0x0000_0000	SLCD WR's duty*/
#define DC_SLCD_WR_DUTY		        (0xA004)
/* RW	32	0x0000_0000	SLCD timing*/
#define DC_SLCD_TIMING		        (0xA008)
/* RW	32	0x0000_0000	frame size*/
#define DC_SLCD_FRM_SIZE	        (0xA00C)
/* RW	32	0x0000_0000	slow time*/
#define DC_SLCD_SLOW_TIME	        (0xA010)
/* RW	32	0x0000_0000	SLCD command*/
#define DC_SLCD_REG_IF			(0xA014)
/* RW	32	0x0000_0000	SLCD status*/
#define DC_SLCD_ST			(0xA018)
#define DC_SLCD_REG_CTRL		(0xA01c)


/*-------------------------------------------------------------------------------
 *			DPU Registers Bits Field Define
 * -----------------------------------------------------------------------------*/

#define BIT(x)				(1 << (x))
/* Frame descriptor control(DC_FRM_CFG_CTRL) bit field define */

/* 1:change to single read channel after the current frame ; 0:no effect */
#define	DC_CHANGE_2_RDMA		BIT(1)
/* Frame descriptor DMA start */
#define	DC_FRM_START			BIT(0)

/* RDMA descriptor control(DC_RDMA_CHAIN_CTRL) bit field define */

/* 1:change to composer channel after the current frame ; 0:no effect */
#define	DC_CHANGE_2_CMP 		BIT(1)
/* RDMA descriptor DMA start */
#define	DC_RDMA_START			BIT(0)

/* Control configure(DC_CTRL) bit field define */

/* General stop the simple read channel.Assure integrity of the current frame */
#define DC_GEN_STP_RDMA			BIT(4)
/* General stop the composer channel. Assure integrity of the current frame */
#define DC_GEN_STP_CMP			BIT(3)
/* Reset the counter of FRM_DES, LAYx_DES and RDMA_DES */
#define DC_DES_CNT_RST			BIT(2)
/* Quick stop the simple read channel. */
#define DC_QCK_STP_RDMA			BIT(1)
/* Quick stop the composer channel. */
#define DC_QCK_STP_CMP			BIT(0)

/* Status Register(DC_ST) bit field define */

/* Composer cannot meet the writeback frame rate*/
#define DC_CMP_W_SLOW			BIT(31)
/* One frame display end */
#define DC_DISP_END			BIT(17)
/* write back DMA is over-run. */
#define DC_WDMA_OVER	 		BIT(16)
/* write back DMA end. */
#define DC_WDMA_END			BIT(15)
/* simple read DMA end. */
#define DC_CMP_START			BIT(14)
/* layer3's block DMA end. */
#define DC_LAY3_END			BIT(13)
/* layer2's block DMA end. */
#define DC_LAY2_END			BIT(12)
/* layer1's block DMA end. */
#define DC_LAY1_END			BIT(11)
/* layer0's block DMA end. */
#define DC_LAY0_END			BIT(10)
/*  composer end of each frame. */
#define DC_CMP_END			BIT(9)
/* TFT jump into under-run mode. */
#define DC_TFT_UNDR			BIT(8)
/* writeback channel is general stop */
#define DC_STOP_WRBK_ACK		BIT(7)
/* Display channel general stop */
#define DC_STOP_DISP_ACK		BIT(6)
/* writeback channel is working */
#define DC_WRBK_WORKING			BIT(5)
/* composer direct output channel is working. */
#define DC_DIRECT_WORKING		BIT(4)
#define DC_SRD_WORKING			BIT(3)
#define DC_SRD_START			BIT(2)
#define DC_SRD_END			BIT(1)
/* display controller is working. */
#define DC_WORKING			BIT(0)

/* Clear status Register(DC_CSR) bit field define */

/* Clear CMP_W_SLOW*/
#define DC_CLR_CMP_W_SLOW		BIT(31)
#define DC_CLR_DISP_END			BIT(17)
/* clear WDMA_OVER */
#define DC_CLR_WDMA_OVER		BIT(16)
/* clear WDMA_END */
#define DC_CLR_WDMA_END			BIT(15)
#define DC_CLR_CMP_START			BIT(14)
/* clear BDMA3_END */
#define DC_CLR_LAY3_END		BIT(13)
/* clear BDMA2_END */
#define DC_CLR_LAY2_END		BIT(12)
/* clear BDMA1_END */
#define DC_CLR_LAY1_END		BIT(11)
/* clear BDMA0_END */
#define DC_CLR_LAY0_END		BIT(10)
/* clear BDMA_END */
#define DC_CLR_CMP_END			BIT(9)
/* clear TFT_UNDR */
#define DC_CLR_TFT_UNDR			BIT(8)
/* clear STOP_WRBK_ACK */
#define DC_CLR_STOP_WRBK_ACK		BIT(7)
/* clear STOP_DISP_ACK */
#define DC_CLR_STOP_DISP_ACK		BIT(6)
#define DC_CLR_SRD_START		BIT(2)
#define DC_CLR_SRD_END			BIT(1)

/* INTC register(DC_INTC) bit field define */

/* Mask of CMP_W_SLOW*/
#define DC_CWS_MSK			BIT(31)
/* mask of DISP_END */
#define DC_EOD_MSK			BIT(17)
/* mask of WDMA_OVER */
#define DC_OOW_MSK			BIT(16)
/* mask of WDMA_END */
#define DC_EOW_MSK			BIT(15)
#define DC_SOC_MSK			BIT(14)
/* mask of BDMA3_END */
#define DC_EOL3_MSK			BIT(13)
/* mask of BDMA2_END */
#define DC_EOL2_MSK			BIT(12)
/* mask of BDMA1_END */
#define DC_EOL1_MSK			BIT(11)
/* mask of BDMA0_END */
#define DC_EOL0_MSK			BIT(10)
#define DC_EOC_MSK			BIT(9)
/* mask of TFT_UNDR */
#define DC_UOT_MSK			BIT(8)
/* mask of STOP_WRBK_ACK */
#define DC_SWA_MSK			BIT(7)
/* mask of STOP_DISP_ACK */
#define DC_SDA_MSK			BIT(6)
#define DC_SOS_MSK			BIT(2)
#define DC_EOS_MSK			BIT(1)

/* DC interrupt flag(DC_INT_FLAG) */

/* Interrupt of CMP_W_SLOW*/
#define DC_INT_CWS			BIT(31)
/* Interrupt of DISP_END */
#define DC_INT_EOD			BIT(17)
#define DC_INT_OOW			BIT(16)
#define DC_INT_EOW			BIT(15)
#define DC_INT_SOC			BIT(14)
#define DC_INT_EOL3			BIT(13)
#define DC_INT_EOL2			BIT(12)
/* Interrupt of BDMA1_END */
#define DC_INT_EOL1			BIT(11)
/* Interrupt of BDMA0_END */
#define DC_INT_EOL0			BIT(10)
/* Interrupt of BDMA_END */
#define DC_INT_EOC			BIT(9)
/* Interrupt of TFT_UNDR */
#define DC_INT_UOT			BIT(8)
#define DC_INT_SWA			BIT(7)
/* Interrupt of STOP_DISP_ACK */
#define DC_INT_SDA			BIT(6)
#define DC_INT_SOS			BIT(2)
#define DC_INT_EOS			BIT(1)



/* Common configure register(DC_COM_CONFIG) bit field define */


/***add reg  clk_gate_en***/
#define DC_BURST_LEN_WDMA_LBIT		(6)
#define DC_BURST_LEN_WDMA_HBIT		(7)
#define DC_BURST_LEN_WDMA_MASK	\
	GENMASK(DC_BURST_LEN_WDMA_HBIT, DC_BURST_LEN_WDMA_LBIT)
#define DC_BURST_LEN_WDMA_4		(0) << DC_BURST_LEN_WDMA_LBIT
#define DC_BURST_LEN_WDMA_8		(1) << DC_BURST_LEN_WDMA_LBIT
#define DC_BURST_LEN_WDMA_16		(2) << DC_BURST_LEN_WDMA_LBIT
#define DC_BURST_LEN_WDMA_32		(3) << DC_BURST_LEN_WDMA_LBIT

#define DC_BURST_LEN_RDMA_LBIT		(4)
#define DC_BURST_LEN_RDMA_HBIT		(5)
#define DC_BURST_LEN_RDMA_MASK	\
	GENMASK(DC_BURST_LEN_RDMA_HBIT, DC_BURST_LEN_RDMA_LBIT)
#define DC_BURST_LEN_RDMA_4		(0) << DC_BURST_LEN_RDMA_LBIT
#define DC_BURST_LEN_RDMA_8		(1) << DC_BURST_LEN_RDMA_LBIT
#define DC_BURST_LEN_RDMA_16		(2) << DC_BURST_LEN_RDMA_LBIT
#define DC_BURST_LEN_RDMA_32		(3) << DC_BURST_LEN_RDMA_LBIT

#define DC_BURST_LEN_BDMA_LBIT		(2)
#define DC_BURST_LEN_BDMA_HBIT		(3)
#define DC_BURST_LEN_BDMA_MASK	\
	GENMASK(DC_BURST_LEN_BDMA_HBIT, DC_BURST_LEN_BDMA_LBIT)
#define DC_BURST_LEN_BDMA_4		(0) << DC_BURST_LEN_BDMA_LBIT
#define DC_BURST_LEN_BDMA_8		(1) << DC_BURST_LEN_BDMA_LBIT
#define DC_BURST_LEN_BDMA_16		(2) << DC_BURST_LEN_BDMA_LBIT
#define DC_BURST_LEN_BDMA_32		(3) << DC_BURST_LEN_BDMA_LBIT

/* keep default value */
#define	DC_OUT_SEL			BIT(1)
#define DC_OUT_SEL_RDMA                 BIT(1)
#define DC_OUT_SEL_CMP			~BIT(1)

/* PCFG_RD_CTRL register */

/*  QoS value */
#define DC_ARQOS_VAL_LBIT		(1)
#define DC_ARQOS_VAL_HBIT		(2)
#define DC_ARQOS_VAL_MASK   \
	GENMASK(DC_ARQOS_VAL_HBIT, DC_ARQOS_VAL_LBIT)
#define DC_ARQOS_VAL_0			(0) << DC_ARQOS_VAL_LBIT
#define DC_ARQOS_VAL_1			(1) << DC_ARQOS_VAL_LBIT
#define DC_ARQOS_VAL_2			(2) << DC_ARQOS_VAL_LBIT
#define DC_ARQOS_VAL_3			(3) << DC_ARQOS_VAL_LBIT
/*  QoS ctrl */
#define DC_ARQOS_CTRL			BIT(0)

/* PCFG register(OFIFO's PCFG Register) bit field define */
#define DC_PCFG0_LBIT			(0)
#define DC_PCFG0_HBIT			(8)
#define DC_PCFG0_MASK	      \
	GENMASK(DC_PCFG0_HBIT, DC_PCFG0_LBIT)

#define DC_PCFG1_LBIT			(9)
#define DC_PCFG1_HBIT			(17)
#define DC_PCFG1_MASK	      \
	GENMASK(DC_PCFG1_HBIT, DC_PCFG1_LBIT)

#define DC_PCFG2_LBIT			(18)
#define DC_PCFG2_HBIT			(26)
#define DC_PCFG2_MASK	       \
	GENMASK(DC_PCFG2_HBIT, DC_PCFG2_LBIT)

/* Global control(GLBC) bit field define */

/* TLB enable */
#define DC_TLBE_LBIT			(0)
#define DC_TLBE_HBIT			(3)
#define DC_TLBE_MASK        \
	GENMASK(DC_TLBE_HBIT, DC_TLBE_LBIT)
/* TLB channel 0 enable */
#define DC_CH0_TLBE 			BIT(0)
/* TLB channel 1 enable */
#define DC_CH1_TLBE 			BIT(1)
/* TLB channel 2 enable */
#define DC_CH2_TLBE 			BIT(2)
/* TLB channel 3 enable */
#define DC_CH3_TLBE 			BIT(3)

/* Interrupt enable */
#define DC_INTE_LBIT			(4)
#define DC_INTE_HBIT			(7)
#define DC_INTE_MASK        \
	GENMASK(DC_INTE_HBIT, DC_INTE_LBIT)
/* TLB channel 0 interrupt enable */
#define DC_CH0_INTE			BIT(4)
/* TLB channel 1 interrupt enable */
#define DC_CH1_INTE			BIT(5)
/* TLB channel 2 interrupt enable */
#define DC_CH2_INTE			BIT(6)
/* TLB channel 3 interrupt enable */
#define DC_CH3_INTE			BIT(7)

/* TLB fetch mode, for STB miss */
#define DC_FMODE_LBIT			(8)
#define DC_FMODE_HBIT			(11)
#define DC_FMODE_MASK        \
	GENMASK(DC_FMODE_HBIT, DC_FMODE_LBIT)
#define DC_CH0_FMODE			BIT(8)
#define DC_CH1_FMODE			BIT(9)
#define DC_CH2_FMODE			BIT(10)
#define DC_CH3_FMODE			BIT(11)

/* TLB table address(GLBA) bit field define */

/* TLB table address,Its only used and must \
 *  be initialized when GLBC.TLBEn was set */
#define DC_TLBA_LBIT			(12)
#define DC_TLBA_HBIT			(31)
#define DC_TLBA_MASK        \
	GENMASK(DC_TLBA_HBIT, DC_TLBA_LBIT)

/* TLB control Trigger(TLBC) bit field define */

/*  When TLB table entry un-usable case occurs, SW set this bit to \
 *  inform to retry reading the TLB table entry again.*/
#define DC_RETRY_LBIT			(4)
#define DC_RETRY_HBIT			(7)
#define DC_RETRY_MASK        \
	GENMASK(DC_RETRY_HBIT, DC_RETRY_LBIT)
#define DC_CH0_RETRY                    BIT(4)
#define DC_CH1_RETRY		        BIT(5)
#define DC_CH2_RETRY                    BIT(6)
#define DC_CH3_RETRY                    BIT(7)

/*  Write 1 to invalidate all TLB caches. */
#define DC_INVLD_LBIT	                (0)
#define DC_INVLD_HBIT		        (3)
#define DC_INVLD_MASK        \
	GENMASK(DC_INVLD_HBIT, DC_INVLD_LBIT)
#define DC_CH0_INVLD         	        BIT(0)
#define DC_CH1_INVLD		        BIT(1)
#define DC_CH2_INVLD                    BIT(2)
#define DC_CH3_INVLD		  	BIT(3)

/* TLB VPNX Status(VPNX) bit field define */

/* When working in TLB enable case, if the relative TLB table
 * entry in the external DDR is un-useable, VPU will record its
 * virtual address bit31:12 into VPN. SW can read-out VPN for
 * TLB table entry,s rebuilding.*/
#define	DC_VPNX_LBIT			(12)
#define DC_VPNX_HBIT			(31)
#define DC_VPNX_MASK		\
	GENMASK(DC_VPNX_HBIT, DC_VPNX_LBIT)

/* TLB table entry verification(TLBV) bit field define */

/* Check bit ignored(ignore the vld bit for PEntry, SEntry in memory*/
#define DC_RCI				BIT(31)
/* Check number mask */
#define DC_CNM_LBIT	                (16)
#define DC_CNM_HBIT                     (27)
#define DC_CNM_MASK		\
	GENMASK(DC_CNM_HBIT, DC_CNM_LBIT)
/* Golden Check number,
 * VPU will check its secondary table entry to judge whether it is valid.
 * If (GCN & CNM) == (TLB entry data readed by TLB [11:0] &CNM)
 * indicates current entry is valid, otherwise it is unusable.*/
#define DC_GCN_LBIT                     (0)
#define DC_GCN_HBIT                     (11)
#define DC_GCN_MASK          \
	GENMASK(DC_GCN_HBIT, DC_GCN_LBIT)

/* SCH status(STAT) bit field define */

/* CH3 TLB error occurs. Write 0 to cleared */
#define DC_CH3_TLB_ERR			BIT(3)
/* CH2 TLB error occurs. Write 0 to cleared */
#define DC_CH2_TLB_ERR			BIT(2)
/* CH1 TLB error occurs. Write 0 to cleared */
#define DC_CH1_TLB_ERR			BIT(1)
/* CH0 TLB error occurs. Write 0 to cleared */
#define DC_CH0_TLB_ERR			BIT(0)

/* CscMultYRv */

#define DC_CSC_MULT_Y_LBIT		(0)
#define DC_CSC_MULT_Y_HBIT		(10)
#define DC_CSC_MULT_Y_MASK        \
	GENMASK(DC_CSC_MULT_Y_HBIT, DC_CSC_MULT_Y_LBIT)
#define DC_CSC_MULT_Y_MD0		(0x400) << DC_CSC_MULT_Y_LBIT
#define DC_CSC_MULT_Y_MD1		(0x4a8) << DC_CSC_MULT_Y_LBIT
#define DC_CSC_MULT_Y_MD2		(0x400) << DC_CSC_MULT_Y_LBIT
#define DC_CSC_MULT_Y_MD3		(0x47c) << DC_CSC_MULT_Y_LBIT

#define DC_CSC_MULT_RV_LBIT		(16)
#define DC_CSC_MULT_RV_HBIT		(26)
#define DC_CSC_MULT_RV_MASK        \
	GENMASK(DC_CSC_MULT_RV_HBIT, DC_CSC_MULT_RV_LBIT)
#define DC_CSC_MULT_RV_MD0		(0x48f) << DC_CSC_MULT_RV_LBIT
#define DC_CSC_MULT_RV_MD1		(0x662) << DC_CSC_MULT_RV_LBIT
#define DC_CSC_MULT_RV_MD2		(0x59c) << DC_CSC_MULT_RV_LBIT
#define DC_CSC_MULT_RV_MD3		(0x57c) << DC_CSC_MULT_RV_LBIT

/* CscMultGuGv */

#define DC_CSC_MULT_GU_LBIT		(0)
#define DC_CSC_MULT_GU_HBIT		(8)
#define DC_CSC_MULT_GU_MASK        \
	GENMASK(DC_CSC_MULT_GU_HBIT, DC_CSC_MULT_GU_LBIT)
#define DC_CSC_MULT_GU_MD0		(0x193) << DC_CSC_MULT_GU_LBIT
#define DC_CSC_MULT_GU_MD1		(0x190) << DC_CSC_MULT_GU_LBIT
#define DC_CSC_MULT_GU_MD2		(0x160) << DC_CSC_MULT_GU_LBIT
#define DC_CSC_MULT_GU_MD3		(0x15a) << DC_CSC_MULT_GU_LBIT

#define DC_CSC_MULT_GV_LBIT		(16)
#define DC_CSC_MULT_GV_HBIT		(25)
#define DC_CSC_MULT_GV_MASK        \
	GENMASK(DC_CSC_MULT_GV_HBIT, DC_CSC_MULT_GV_LBIT)
#define DC_CSC_MULT_GV_MD0		(0x253) << DC_CSC_MULT_GV_LBIT
#define DC_CSC_MULT_GV_MD1		(0x341) << DC_CSC_MULT_GV_LBIT
#define DC_CSC_MULT_GV_MD2		(0x2db) << DC_CSC_MULT_GV_LBIT
#define DC_CSC_MULT_GV_MD3		(0x2cb) << DC_CSC_MULT_GV_LBIT

/* CscMultBu */

#define DC_CSC_MULT_BU_LBIT		(0)
#define DC_CSC_MULT_BU_HBIT		(11)
#define DC_CSC_MULT_BU_MASK        \
	GENMASK(DC_CSC_MULT_BU_HBIT, DC_CSC_MULT_BU_LBIT)
#define DC_CSC_MULT_BU_MD0		(0x820) << DC_CSC_MULT_BU_LBIT
#define DC_CSC_MULT_BU_MD1		(0x812) << DC_CSC_MULT_BU_LBIT
#define DC_CSC_MULT_BU_MD2		(0x717) << DC_CSC_MULT_BU_LBIT
#define DC_CSC_MULT_BU_MD3		(0x6ee) << DC_CSC_MULT_BU_LBIT

/* CscSubYUV */

#define DC_CSC_SUB_Y_LBIT		(0)
#define DC_CSC_SUB_Y_HBIT		(4)
#define DC_CSC_SUB_Y_MASK        \
	GENMASK(DC_CSC_SUB_Y_HBIT, DC_CSC_SUB_Y_LBIT)
#define DC_CSC_SUB_Y_MD0		(0x00) << DC_CSC_SUB_Y_LBIT
#define DC_CSC_SUB_Y_MD1		(0x10) << DC_CSC_SUB_Y_LBIT
#define DC_CSC_SUB_Y_MD2		(0x00) << DC_CSC_SUB_Y_LBIT
#define DC_CSC_SUB_Y_MD3		(0x00) << DC_CSC_SUB_Y_LBIT

#define DC_CSC_SUB_UV_LBIT		(16)
#define DC_CSC_SUB_UV_HBIT		(23)
#define DC_CSC_SUB_UV_MASK        \
	GENMASK(DC_CSC_SUB_UV_HBIT, DC_CSC_SUB_UV_LBIT)
#define DC_CSC_SUB_UV_MD0		(0x00) << DC_CSC_SUB_UV_LBIT
#define DC_CSC_SUB_UV_MD1		(0x80) << DC_CSC_SUB_UV_LBIT
#define DC_CSC_SUB_UV_MD2		(0x80) << DC_CSC_SUB_UV_LBIT
#define DC_CSC_SUB_UV_MD3		(0x80) << DC_CSC_SUB_UV_LBIT

/* Common(DISP_COM) bit field define */

/* Dither drop bit. */
#define DC_DP_DITHER_DW_LBIT		(16)
#define DC_DP_DITHER_DW_HBIT		(21)
#define DC_DP_DITHER_DW_MASK        \
	GENMASK(DC_DP_DITHER_DW_HBIT, DC_DP_DITHER_DW_LBIT)
#define DC_DP_DITHER_DW_BLUE_LBIT       (16)
#define DC_DP_DITHER_DW_GREEN_LBIT      (18)
#define DC_DP_DITHER_DW_RED_LBIT        (20)
#define DC_DP_DITHER_DROP_0_BITS	(0)
#define DC_DP_DITHER_DROP_2_BITS	(1)
#define DC_DP_DITHER_DROP_3_BITS	(2)
#define DC_DP_DITHER_DROP_4_BITS	(3)
#define	DC_RGB888_TO_RGB565_DITHER	(0b100110) << DC_DP_DITHER_DW_LBIT
#define	DC_RGB888_TO_RGB666_DITHER	(0b010101) << DC_DP_DITHER_DW_LBIT
/* Dither Clk gate enable */
#define DC_DITHER_CLKGATE_EN		BIT(4)
/* SLCD Clk gate enable */
#define DC_SLCD_CLKGATE_EN			BIT(3)
/* TFT Clk gate enable */
#define DC_TFT_CLKGATE_EN			BIT(2)
/* Dither enable. 1:dither enable 0:dither disable */
#define DC_DP_DITHER_EN			BIT(4)
/* Display interfaces select. 1:TFT 2:SLCD */

#define DC_DP_IF_SEL_LBIT		(0)
#define DC_DP_IF_SEL_HBIT		(1)
#define DC_DP_IF_SEL_MASK		GENMASK(DC_DP_IF_SEL_HBIT, DC_DP_IF_SEL_LBIT)
#define	DC_DISP_COM_NO_DISP		(0b00) << DC_DP_IF_SEL_LBIT
#define	DC_DISP_COM_TFT			(0b01) << DC_DP_IF_SEL_LBIT
#define	DC_DISP_COM_SLCD		(0b10) << DC_DP_IF_SEL_LBIT
#define	DC_DISP_COM_MIPI_SLCD		(0b11) << DC_DP_IF_SEL_LBIT

/* TIMING_HSYNC(TFT_TIMING_HSYNC) bit field define */

/* HSYNC pulse start point(base on dot clock cycle) */
#define DC_HPS_LBIT			(16)
#define DC_HPS_HBIT			(27)
#define DC_HPS_MASK           \
	GENMASK(DC_HPS_HBIT, DC_HPS_LBIT)
/* HSYNC pulse end point(base on dot clock cycle) */
#define DC_HPE_LBIT			(0)
#define DC_HPE_HBIT			(11)
#define DC_HPE_MASK           \
	GENMASK(DC_HPE_HBIT, DC_HPE_LBIT)

/* TIMING_VSYNC(TFT_TIMING_VSYNC) bit field define */

/* VSYNC pulse start point(base on dot clock cycle) */
#define DC_VPS_LBIT			(16)
#define DC_VPS_HBIT			(27)
#define DC_VPS_MASK           \
	GENMASK(DC_VPS_HBIT, DC_VPS_LBIT)
/* VSYNC pulse end point(base on dot clock cycle) */
#define DC_VPE_LBIT			(0)
#define DC_VPE_HBIT			(11)
#define DC_VPE_MASK           \
	GENMASK(DC_VPE_HBIT, DC_VPE_LBIT)

/* TIMING_HDE(TFT_TIMING_HDE) bit field define */

/* Horizontal display area start point (base on dot clock cycle).
 * And it is also the DE pulse start point in horizontal.
 * It indicate the end point of vertical back porch.*/
#define DC_HDS_LBIT			(16)
#define DC_HDS_HBIT			(27)
#define DC_HDS_MASK              \
	GENMASK(HDE_HDS_HBIT, DC_HDS_LBIT)
/* Horizontal display area end point (base on dot clock cycle).
 * And it is also the DE pulse end point in horizontal.
 * It indicate the end point of horizontal front porch.*/
#define DC_HDE_LBIT			(0)
#define DC_HDE_HBIT			(11)
#define DC_HDE_MASK              \
	GENMASK(DC_HDE_HBIT, DC_HDE_LBIT)

/* TIMING_VDE(TFT_TIMING_VDE) bit field define */

/* Vertical display area start point(base on dot clock cycle).
 * And it is also the DE pulse start point in vertical.
 * It indicate the end point of vertical back porch. */
#define DC_VDS_LBIT			(16)
#define DC_VDS_HBIT			(27)
#define DC_VDS_MASK               \
	GENMASK(VDE_VDS_HBIT, DC_VDS_LBIT)
/*  Vertical display area end point(base on dot clock cycle).
 *  And it is also the DE pulse end point in vertical.
 *  It indicate the start point of vertical front porch. */
#define DC_VDE_LBIT			(0)
#define DC_VDE_HBIT			(11)
#define DC_VDE_MASK               \
	GENMASK(DC_VDE_HBIT, DC_VDE_LBIT)

/* TRANS_CONFIG(TFT_TRAN_CFG) bit field define */

/* Inverse the polarity of the DOT_CLK */
#define	DC_PIX_CLK_INV			BIT(10)
/* Default level of DE(invalid level) */
#define	DC_DE_DL			BIT(9)
/* Default level of HSYNC and VSYNC(invalid level) */
#define DC_SYNC_DL			BIT(8)
/* Output color mode of even lines(2,4,6...) */
#define DC_COLOR_EVEN_LBIT		(19)
#define DC_COLOR_EVEN_HBIT		(21)
#define DC_COLOR_EVEN_MASK        \
	GENMASK(DC_COLOR_EVEN_HBIT, DC_COLOR_EVEN_LBIT)
#define DC_EVEN_RGB             	(0b000) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_RBG             	(0b001) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_BGR             	(0b010) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_BRG             	(0b011) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_GBR             	(0b100) << DC_COLOR_EVEN_LBIT
#define DC_EVEN_GRB             	(0b101) << DC_COLOR_EVEN_LBIT
/* Output color mode of odd lines(1, 3, 5...) */
#define DC_COLOR_ODD_LBIT		(16)
#define DC_COLOR_ODD_HBIT		(18)
#define DC_COLOR_ODD_MASK          \
	GENMASK(DC_COLOR_ODD_HBIT, DC_COLOR_ODD_LBIT)
#define DC_ODD_RGB			(0b000) << DC_COLOR_ODD_LBIT
#define DC_ODD_RBG			(0b001) << DC_COLOR_ODD_LBIT
#define DC_ODD_BGR			(0b010) << DC_COLOR_ODD_LBIT
#define DC_ODD_BRG			(0b011) << DC_COLOR_ODD_LBIT
#define DC_ODD_GBR			(0b100) << DC_COLOR_ODD_LBIT
#define DC_ODD_GRB			(0b101) << DC_COLOR_ODD_LBIT
/* 0x:0x: 24bits parallel mode
 * 10: 8bits serial mode without dummy(RGB)
 * 11: 8bits serial mode with dummy(RGBD)*/
#define DC_MODE_LBIT			(0)
#define DC_MODE_HBIT		        (2)
#define DC_MODE_MASK               \
	GENMASK(DC_MODE_HBIT, DC_MODE_LBIT)
#define DC_MODE_PARALLEL_888		(0b000) << DC_MODE_LBIT
#define DC_MODE_PARALLEL_666		(0b001) << DC_MODE_LBIT
#define DC_MODE_PARALLEL_565		(0b010) << DC_MODE_LBIT
#define DC_MODE_SERIAL_8BIT_RGB	        (0b100) << DC_MODE_LBIT
#define DC_MODE_SERIAL_8BIT_RGBD	(0b101) << DC_MODE_LBIT

/* STATUS(TFT_ST) bit field define */

/* 0: TFT is not working 1: TFT is working */
#define	DC_TFT_WORKING			BIT(1)
/* Indicate the controller jump into recovery state,
 * and starting trans one frame 0 to panel.
 * 0: TFT is working order 1: TFT is in UNDER mode.*/
#define	DC_TFT_UNDER			BIT(0)

/* SLCD_CONFIG(SLCD_PANEL_CFG) bit field define */

/* Anti-jitter for RDY.
 * 0: sample RDY with 1 pixclk cycle(default)
 * 1: sample RDY with 3 pixclk cycles for anti-jitter.*/
#define	DC_RDY_ANTI_JIT			BIT(27)
/* Whether the pixel data or command need conversion
 * 0: no use conversion, such as parameter of command(default)
 * 1: need conversion, such as pixel data*/
#define	DC_FMT_EN			BIT(26)
/*  001: typeA(6800) 010: typeB(8080)(default) 100: typeC(SPI) */
#define DC_DBI_TYPE_LBIT		(23)
#define DC_DBI_TYPE_HBIT		(25)
#define DC_DBI_TYPE_MASK        \
	GENMASK(DC_DBI_TYPE_HBIT, DC_DBI_TYPE_LBIT)
#define	DC_DBI_TYPE_A_6800		(0b001) << DC_DBI_TYPE_LBIT
#define	DC_DBI_TYPE_B_8080		(0b010) << DC_DBI_TYPE_LBIT
#define DC_DBI_TYPE_C_SPI_3             (0b100) << DC_DBI_TYPE_LBIT
#define DC_DBI_TYPE_C_SPI_4             (0b101) << DC_DBI_TYPE_LBIT
/* Color format.
 * 00: 565  01: 666 10: 888(default) */
#define DC_DATA_FMT_LBIT		(21)
#define DC_DATA_FMT_HBIT		(22)
#define DC_DATA_FMT_MASK        \
	GENMASK(DC_DATA_FMT_HBIT, DC_DATA_FMT_LBIT)
#define DC_DATA_FMT_565			(0b00) << DC_DATA_FMT_LBIT
#define DC_DATA_FMT_444			(0b01) << DC_DATA_FMT_LBIT
#define DC_DATA_FMT_666			(0b01) << DC_DATA_FMT_LBIT
#define DC_DATA_FMT_888			(0b10) << DC_DATA_FMT_LBIT
/*  Anti-jitter for TE.
 *  0: sample TE with 1 pixclk cycle;
 *  1: sample TE with 3 pixclk cycles for anti-jitter(default).*/
#define	DC_TE_ANTI_JIT	                BIT(20)
/* The active edge of TE.
 * 0: the front edge(default) 1: the back edge*/
#define	DC_TE_MD			BIT(19)
/* 0: do not wait TE, send pix_data after SLCD_START;
 * 1: wait TE, and then send pix_data.(default)*/
#define	DC_TE_SWITCH	        	BIT(18)
/* 0: do not wait RDY, send command or data immediately;(default)
 * 1: wait RDY, and then send command or data. */
#define	DC_RDY_SWITCH			BIT(17)
/* CS control enable.
 * 0: The CS pin is controlled by GPIO.(default)
 * 1: The CS pin is controlled by Display Controller. */
#define	DC_CS_EN			BIT(16)
/* The CS's default polarity
 * 0: the default(invalid) level is low;
 * 1: the default(invalid) level is high;(default) */
#define	DC_CS_DP			BIT(11)
/* The RDY's default polarity
 * 0: the default(invalid) level is low;
 * 1: the default(invalid) level is high;(default) */
#define	DC_RDY_DP			BIT(10)
/* DC's definition.
 * 0: Command DC = low, Data DC = high(default)
 * 1: Command DC = high, Data DC = low */
#define	DC_DC_MD			BIT(9)
/* WR's default polarity.
 * 0: drive at posedge, and sample at negedge;
 * 1: drive at negedge, and sample at posedge.(default) */
#define	DC_WR_DP			BIT(8)
///* SPI_CLK Polarity.
// * 0: Active edge is Falling(default)
// * 1: Active edge is Rising Used for serial transfer mode. */
//#define	DC_CLKPLY			BIT(7)

#define DC_TE_MIPI_SWITCH		BIT(7)

/* The TE' default polarity
 * 0: the default(invalid) level is low;(default)
 * 1: the default(invalid) level is high; */
#define	DC_TE_DP			BIT(6)
/* Panel's data width. It's also used in MIPI DSI command mod
 * 000: 8bits 001: 9bits 010: 16bits 011: 18bits 100: 24bits(default)  */
#define DC_DWIDTH_LBIT			(3)
#define DC_DWIDTH_HBIT			(5)
#define DC_DWIDTH_MASK        \
	GENMASK(DC_DWIDTH_HBIT, DC_DWIDTH_LBIT)
#define	DC_DWIDTH_8BITS			(0b000) << DC_DWIDTH_LBIT
#define	DC_DWIDTH_9BITS			(0b001) << DC_DWIDTH_LBIT
#define	DC_DWIDTH_16BITS		(0b010) << DC_DWIDTH_LBIT
#define	DC_DWIDTH_18BITS		(0b011) << DC_DWIDTH_LBIT
#define	DC_DWIDTH_24BITS		(0b100) << DC_DWIDTH_LBIT
/* The width of panel's command and it's parameters
 * 000: 8bits(default) 001: 9bits 010: 16bits 011: 18bits 100: 24bits */
#define DC_CWIDTH_LBIT			(0)
#define DC_CWIDTH_HBIT			(2)
#define DC_CWIDTH_MASK        \
	GENMASK(DC_CWIDTH_HBIT, DC_CWIDTH_LBIT)
#define	DC_CWIDTH_8BITS			(0b000) << DC_CWIDTH_LBIT
#define	DC_CWIDTH_9BITS			(0b001) << DC_CWIDTH_LBIT
#define	DC_CWIDTH_16BITS		(0b010) << DC_CWIDTH_LBIT
#define	DC_CWIDTH_18BITS		(0b011) << DC_CWIDTH_LBIT
#define	DC_CWIDTH_24BITS		(0b100) << DC_CWIDTH_LBIT

/* SLCD_WR_DUTY(SLCD_WR_DUTY) bit field define */

/* WR's high time period when transmitting data
 * (base on MIPI DBI Type B, and this is low time at Type A) */
#define DC_DSTIME_LBIT			(24)
#define DC_DSTIME_HBIT			(31)
#define DC_DSTIME_MASK        \
	GENMASK(DC_DSTIME_HBIT, DC_DSTIME_LBIT)
/* WR's low time period when transmitting data
 * (base on MIPI DBI Type B, and this is high time at Type A) */
#define DC_DDTIME_LBIT			(16)
#define DC_DDTIME_HBIT			(23)
#define DC_DDTIME_MASK        \
	GENMASK(DC_DDTIME_HBIT, DC_DDTIME_LBIT)
/* WR's high time period when transmitting command
 * (base on MIPI DBI Type B, and this is low time at Type A) */
#define DC_CSTIME_LBIT			(8)
#define DC_CSTIME_HBIT			(15)
#define DC_CSTIME_MASK        \
	GENMASK(DC_CSTIME_HBIT, DC_CSTIME_LBIT)
/* WR's low time period when transmitting command
 * (base on MIPI DBI Type B, and this is high time at Type A) */
#define DC_CDTIME_LBIT			(0)
#define DC_CDTIME_HBIT			(7)
#define DC_CDTIME_MASK        \
	GENMASK(DC_CDTIME_HBIT, DC_CDTIME_LBIT)

/* SLCD_TIMING(SLCD_TIMING) bit field define */
#define DC_TCH_LBIT			(24)
#define DC_TCH_HBIT			(31)
#define DC_TCH_MASK        \
	GENMASK(DC_TCH_HBIT, DC_TCH_LBIT)
#define DC_TCS_LBIT			(16)
#define DC_TCS_HBIT			(23)
#define DC_TCS_MASK        \
	GENMASK(DC_TCS_HBIT, DC_TCS_LBIT)
#define DC_TAH_LBIT			(8)
#define DC_TAH_HBIT			(15)
#define DC_TAH_MASK        \
	GENMASK(DC_TAH_HBIT, DC_TAH_LBIT)
#define DC_TAS_LBIT			(0)
#define DC_TAS_HBIT			(7)
#define DC_TAS_MASK        \
	GENMASK(DC_TAS_HBIT, DC_TAS_LBIT)

/* SLCD_FRM_SIZE(SLCD_FRM_SIZE) bit field define */

/* Line number of each frame. */
#define DC_SLCD_FRM_V_SIZE_LBIT		(16)
#define DC_SLCD_FRM_V_SIZE_HBIT		(31)
#define DC_SLCD_FRM_V_SIZE_MASK        \
	GENMASK(DC_SLCD_FRM_V_SIZE_HBIT, DC_SLCD_FRM_V_SIZE_LBIT)
/* Pixel number of each line. */
#define DC_SLCD_FRM_H_SIZE_LBIT		(0)
#define DC_SLCD_FRM_H_SIZE_HBIT		(15)
#define DC_SLCD_FRM_H_SIZE_MASK        \
	GENMASK(DC_SLCD_FRM_H_SIZE_HBIT, DC_SLCD_FRM_H_SIZE_LBIT)

/* SLCD_SLOW_TIME(SLCD_SLOW_TIME) bit field define */

/* SLCD will delay some period of pixclk before transmitting data,
 * after TE鈥檚 relative edge. */
#define DC_SLOW_TIME_LBIT			(0)
#define DC_SLOW_TIME_HBIT			(15)
#define DC_SLOW_TIME_MASK         \
	GENMASK(DC_SLOW_TIME_HBIT, DC_SLOW_TIME_LBIT)

/* SLCD_REG_IF(SLCD_REG_IF) bit field define */

/* It is used to decide the meanings of the DATA/CMD/PARAMETER.
 * 00: pix_data; 01: parameter; 1x: command(default). */
#define DC_SLCD_REG_IF_FLAG_LBIT		(30)
#define DC_SLCD_REG_IF_FLAG_HBIT		(31)
#define DC_SLCD_REG_IF_FLAG_MASK       \
	GENMASK(DC_SLCD_REG_IF_FLAG_HBIT, DC_SLCD_REG_IF_FLAG_LBIT)
#define DC_SLCD_REG_IF_FLAG_DATA		(0b00) << DC_SLCD_REG_IF_FLAG_LBIT
#define DC_SLCD_REG_IF_FLAG_PRM			(0b01) << DC_SLCD_REG_IF_FLAG_LBIT
#define DC_SLCD_REG_IF_FLAG_CMD			(0b10) << DC_SLCD_REG_IF_FLAG_LBIT
/* Indicate this is the last content, the hardware will disable the CS */
#define	DC_SLCD_REG_IF_END		        BIT(29)
/* content */
#define DC_SLCD_REG_IF_CONTENT_LBIT	(0)
#define DC_SLCD_REG_IF_CONTENT_HBIT	(23)
#define DC_SLCD_REG_IF_CONTENT_MASK     \
	GENMASK(DC_SLCD_REG_IF_CONTENT_HBIT, DC_SLCD_REG_IF_CONTENT_LBIT)

/* SLCD_STATUS(SLCD_ST) bit field define */

/* Indicate whether the SLCD DBI interface controller is working or not.
 * 0: idle 1: busy */
#define DC_SLCD_ST_BUSY			BIT(0)

#endif /* _DPU_REG_H_ */

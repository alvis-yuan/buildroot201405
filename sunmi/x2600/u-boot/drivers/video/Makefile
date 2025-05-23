#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libvideo.o

COBJS-$(CONFIG_ATI_RADEON_FB) += ati_radeon_fb.o videomodes.o
COBJS-$(CONFIG_ATMEL_HLCD) += atmel_hlcdfb.o
COBJS-$(CONFIG_ATMEL_LCD) += atmel_lcdfb.o
COBJS-$(CONFIG_CFB_CONSOLE) += cfb_console.o
COBJS-$(CONFIG_EXYNOS_DP) += exynos_dp.o exynos_dp_lowlevel.o
COBJS-$(CONFIG_EXYNOS_FB) += exynos_fb.o exynos_fimd.o
COBJS-$(CONFIG_EXYNOS_MIPI_DSIM) += exynos_mipi_dsi.o exynos_mipi_dsi_common.o \
				exynos_mipi_dsi_lowlevel.o
COBJS-$(CONFIG_EXYNOS_PWM_BL) += exynos_pwm_bl.o
COBJS-$(CONFIG_FSL_DIU_FB) += fsl_diu_fb.o videomodes.o
COBJS-$(CONFIG_MPC8XX_LCD) += mpc8xx_lcd.o
COBJS-$(CONFIG_PXA_LCD) += pxa_lcd.o
COBJS-$(CONFIG_S6E8AX0) += s6e8ax0.o
COBJS-$(CONFIG_S6E63D6) += s6e63d6.o
COBJS-$(CONFIG_LD9040) += ld9040.o
COBJS-$(CONFIG_SED156X) += sed156x.o
COBJS-$(CONFIG_VIDEO_BCM2835) += bcm2835.o
COBJS-$(CONFIG_VIDEO_COREBOOT) += coreboot_fb.o
COBJS-$(CONFIG_VIDEO_CT69000) += ct69000.o videomodes.o
COBJS-$(CONFIG_VIDEO_DA8XX) += da8xx-fb.o videomodes.o
COBJS-$(CONFIG_VIDEO_MB862xx) += mb862xx.o videomodes.o
COBJS-$(CONFIG_VIDEO_MB86R0xGDC) += mb86r0xgdc.o videomodes.o
COBJS-$(CONFIG_VIDEO_MX3) += mx3fb.o videomodes.o
COBJS-$(CONFIG_VIDEO_IPUV3) += mxc_ipuv3_fb.o ipu_common.o ipu_disp.o
COBJS-$(CONFIG_VIDEO_MXS) += mxsfb.o videomodes.o
COBJS-$(CONFIG_VIDEO_OMAP3) += omap3_dss.o
COBJS-$(CONFIG_VIDEO_SED13806) += sed13806.o
COBJS-$(CONFIG_VIDEO_SM501) += sm501.o
COBJS-$(CONFIG_VIDEO_SMI_LYNXEM) += smiLynxEM.o videomodes.o
COBJS-$(CONFIG_VIDEO_TEGRA) += tegra.o
COBJS-$(CONFIG_VIDEO_VCXK) += bus_vcxk.o
COBJS-$(CONFIG_VIDEO_BYD_BM8766U) += jz_lcd/lcd_panel/byd_bm8766u.o
COBJS-$(CONFIG_VIDEO_BM347WV_F_8991FTGF) += jz_lcd/lcd_panel/byd_8991.o \
	jz_lcd/lcd_panel/byd_8991_init.o
COBJS-$(CONFIG_VIDEO_ILI_6122) += jz_lcd/lcd_panel/ili_6122.o

COBJS-$(CONFIG_VIDEO_X163) += jz_lcd/lcd_panel/auo_x163.o \
				jz_lcd/lcd_panel/write_command_auo_x163.o

COBJS-$(CONFIG_VIDEO_ET017QG1) += jz_epd/epd_panel/ET017QG1/lcd-ET017QG1.o \
								  jz_epd/epd_panel/ET017QG1/PVI_ET017QC1.o \
								  jz_epd/epd_panel/ET017QG1/V220_C157_17_WNA331_ET017QC1_CTC.o

COBJS-$(CONFIG_VIDEO_ED060XD1) += jz4775_epd/epd_panel/ED060XD1/lcd-ED060XD1.o  \
								  jz4775_epd/epd_panel/ED060XD1/PVI_ED060XD1.o  \
								  jz4775_epd/epd_panel/ED060XD1/PVI110_B114_ED060SC4.o
COBJS-$(CONFIG_VIDEO_KFM701A_21_1A) += jz_lcd/lcd_panel/kfm701a_21_1a.o
COBJS-$(CONFIG_VIDEO_BYD_9177AA) += jz_lcd/lcd_panel/byd_9177aa.o
COBJS-$(CONFIG_VIDEO_TRULY_TFT240240_2_E) += jz_lcd/lcd_panel/truly_tft240240_2_e.o
COBJS-$(CONFIG_VIDEO_LCD_Y88249) += jz_lcd/lcd_panel/lcd_y88249.o
COBJS-$(CONFIG_VIDEO_NHD_160128UGC3) += jz_lcd/lcd_panel/nhd_160128ugc.o
COBJS-$(CONFIG_VIDEO_BOE_TFT320320) += jz_lcd/lcd_panel/boe_tft320320.o
COBJS-$(CONFIG_VIDEO_EDO_E1392AM1) += jz_lcd/lcd_panel/edo_e1392am1.o
COBJS-$(CONFIG_VIDEO_LH155) += jz_lcd/lcd_panel/lh155.o
COBJS-$(CONFIG_VIDEO_SPILCD_RRJ_R108103_GC9203_128x220) += jz_lcd/lcd_panel/spilcd_gc9203.o
COBJS-$(CONFIG_VIDEO_MA0060) += jz_lcd/lcd_panel/lcd_ma0060.o
COBJS-$(CONFIG_VIDEO_FW050) += jz_lcd/lcd_panel/lcd_fw050.o
COBJS-$(CONFIG_VIDEO_FW035) += jz_lcd/lcd_panel/lcd_fw035.o
COBJS-$(CONFIG_VIDEO_ST7789V) += jz_lcd/lcd_panel/lcd_st7798v.o
COBJS-$(CONFIG_JZ_MIPI_DSI) += jz_lcd/jz_mipi_dsi/jz_mipi_dsi.o jz_lcd/jz_mipi_dsi/jz_mipi_dsi_lowlevel.o jz_lcd/jz_mipi_dsi/jz_mipi_dsih_hal.o \
				jz_lcd/jz_mipi_dsi/write_command_to_mipi.o
COBJS-$(CONFIG_VIDEO_JZ4775) += jz_lcd/backlight_pwm.o jz_lcd/jz4775_lcd.o
COBJS-$(CONFIG_VIDEO_JZ4780) += jz_lcd/backlight_pwm.o jz_lcd/jz4780_lcd.o
COBJS-$(CONFIG_JZ_LCD_V12) += jz_lcd/backlight_pwm.o jz_lcd/jz_lcd_v12.o
COBJS-$(CONFIG_JZ_LCD_V13) += jz_lcd/backlight_pwm.o jz_lcd/jz_lcd_v13.o
COBJS-$(CONFIG_JZ_LCD_V14) += jz_lcd/backlight_pwm.o jz_lcd/jz_lcd_v14.o
COBJS-$(CONFIG_JZ47xx_EPD) += jz4775_epd/jz4775_epd.o
COBJS-$(CONFIG_JZ_EPD) += jz_epd/jz_epd_v12.o
COBJS-$(CONFIG_JZ_FELIX) += 	jz_vpu/jz_felix_drv.o 			\
				jz_vpu/libh264/api/jzm_h264_dec.o 	\
				jz_vpu/libh264/src/buffer.o	 	\
				jz_vpu/libh264/src/golomb.o 		\
				jz_vpu/libh264/src/h2645_parse.o 	\
				jz_vpu/libh264/src/h264_direct.o 	\
				jz_vpu/libh264/src/h264_parse.o 	\
				jz_vpu/libh264/src/h264_picture.o 	\
				jz_vpu/libh264/src/h264_ps.o 		\
				jz_vpu/libh264/src/h264_refs.o 		\
				jz_vpu/libh264/src/h264_sei.o 		\
				jz_vpu/libh264/src/h264_slice.o 	\
				jz_vpu/libh264/src/h264data.o 		\
				jz_vpu/libh264/src/h264dec.o 		\
				jz_vpu/libh264/src/log.o 		\
				jz_vpu/libh264/src/mathtables.o 	\
				jz_vpu/libh264/src/mem.o 		\
				jz_vpu/libh264/src/vpu_ops.o

ifdef CONFIG_JZ_FELIX
LOCAL_CFLAGS += -I$(TOPDIR)/drivers/video/jz_vpu/ -I$(TOPDIR)/drivers/video/jz_vpu/libh264/ -I$(TOPDIR)/drivers/video/jz_vpu/libh264/include
CFLAGS  += $(LOCAL_CFLAGS)
CPPFLAGS += $(LOCAL_CFLAGS)
endif


COBJS	:= $(sort $(COBJS-y))
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)


$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################

#
# (C) Copyright 2004-2006
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

LIB	= $(obj)libcommon.o

# core
ifndef CONFIG_SPL_BUILD
COBJS-y += main.o
COBJS-y += command.o
COBJS-y += exports.o
COBJS-y += hash.o
COBJS-$(CONFIG_SYS_HUSH_PARSER) += hush.o
COBJS-y += s_record.o
COBJS-$(CONFIG_USE_XYZMODEM) += xyzModem.o
COBJS-y += cmd_disk.o

# boards
COBJS-$(CONFIG_SYS_GENERIC_BOARD) += board_f.o
COBJS-$(CONFIG_SYS_GENERIC_BOARD) += board_r.o

# core command
COBJS-y += cmd_boot.o
COBJS-y += cmd_boota.o
COBJS-$(CONFIG_BOOT_XIMAGE) += cmd_bootx.o
COBJS-$(CONFIG_BOOT_ANDROID) += boot_mode.o
COBJS-$(CONFIG_CMD_FASTBOOT) += cmd_fastboot.o
COBJS-$(CONFIG_CMD_USBSERIAL) += cmd_usbserial.o
COBJS-$(CONFIG_CMD_USBPRINTER) += cmd_usbprinter.o
COBJS-$(CONFIG_CMD_BOOTM) += cmd_bootm.o
COBJS-y += cmd_help.o
COBJS-y += cmd_version.o
COBJS-$(CONFIG_DDR_TEST) += cmd_ddr_test.o

# environment
COBJS-y += env_attr.o
COBJS-y += env_callback.o
COBJS-y += env_flags.o
COBJS-$(CONFIG_FPGA) += cmd_jzfpga.o
COBJS-$(CONFIG_ENV_IS_IN_DATAFLASH) += env_dataflash.o
COBJS-$(CONFIG_ENV_IS_IN_EEPROM) += env_eeprom.o
XCOBJS-$(CONFIG_ENV_IS_EMBEDDED) += env_embedded.o
COBJS-$(CONFIG_ENV_IS_IN_EEPROM) += env_embedded.o
XCOBJS-$(CONFIG_ENV_IS_IN_FLASH) += env_embedded.o
COBJS-$(CONFIG_ENV_IS_IN_NVRAM) += env_embedded.o
COBJS-$(CONFIG_ENV_IS_IN_FLASH) += env_flash.o
COBJS-$(CONFIG_ENV_IS_IN_MMC) += env_mmc.o
COBJS-$(CONFIG_ENV_IS_IN_SFC) += env_sfc.o
COBJS-$(CONFIG_ENV_IS_IN_FAT) += env_fat.o
COBJS-$(CONFIG_ENV_IS_IN_NAND) += env_nand.o
COBJS-$(CONFIG_ENV_IS_IN_SPI_NAND) += env_spinand.o
COBJS-$(CONFIG_ENV_IS_IN_SFC_NAND) += env_sfcnand.o
COBJS-$(CONFIG_ENV_IS_IN_NAND_ZM) += env_nand_zm.o
COBJS-$(CONFIG_ENV_IS_IN_NAND_JZMTD) += env_nand_jzmtd.o
COBJS-$(CONFIG_ENV_IS_IN_NVRAM) += env_nvram.o
COBJS-$(CONFIG_ENV_IS_IN_ONENAND) += env_onenand.o
COBJS-$(CONFIG_ENV_IS_IN_SPI_FLASH) += env_sf.o
COBJS-$(CONFIG_ENV_IS_IN_REMOTE) += env_remote.o
COBJS-$(CONFIG_ENV_IS_IN_UBI) += env_ubi.o
COBJS-$(CONFIG_ENV_IS_NOWHERE) += env_nowhere.o
COBJS-$(CONFIG_NET_NAND_MAC) += gmac_interface.o

# command
COBJS-$(CONFIG_CMD_AMBAPP) += cmd_ambapp.o
COBJS-$(CONFIG_SOURCE) += cmd_source.o
COBJS-$(CONFIG_CMD_SOURCE) += cmd_source.o
COBJS-$(CONFIG_CMD_BDI) += cmd_bdinfo.o
COBJS-$(CONFIG_CMD_BEDBUG) += bedbug.o cmd_bedbug.o
COBJS-$(CONFIG_CMD_BMP) += cmd_bmp.o
COBJS-$(CONFIG_CMD_JZ_VPU_TEST) += cmd_jz_vpu.o
COBJS-$(CONFIG_CMD_LOGO_RLE) += cmd_rle.o
COBJS-$(CONFIG_CMD_BOOTMENU) += cmd_bootmenu.o
COBJS-$(CONFIG_CMD_BOOTLDR) += cmd_bootldr.o
COBJS-$(CONFIG_CMD_BOOTSTAGE) += cmd_bootstage.o
COBJS-$(CONFIG_CMD_CACHE) += cmd_cache.o
COBJS-$(CONFIG_CMD_CBFS) += cmd_cbfs.o
COBJS-$(CONFIG_CMD_CONSOLE) += cmd_console.o
COBJS-$(CONFIG_CMD_CPLBINFO) += cmd_cplbinfo.o
COBJS-$(CONFIG_DATAFLASH_MMC_SELECT) += cmd_dataflash_mmc_mux.o
COBJS-$(CONFIG_CMD_DATE) += cmd_date.o
COBJS-$(CONFIG_CMD_SOUND) += cmd_sound.o
COBJS-$(CONFIG_VIBRATE_REGULATOR) += cmd_vibrator_shock.o
ifdef CONFIG_4xx
COBJS-$(CONFIG_CMD_SETGETDCR) += cmd_dcr.o
endif
ifdef CONFIG_POST
COBJS-$(CONFIG_CMD_DIAG) += cmd_diag.o
endif
COBJS-$(CONFIG_CMD_DISPLAY) += cmd_display.o
COBJS-$(CONFIG_CMD_DTT) += cmd_dtt.o
COBJS-$(CONFIG_CMD_ECHO) += cmd_echo.o
COBJS-$(CONFIG_ENV_IS_IN_EEPROM) += cmd_eeprom.o
COBJS-$(CONFIG_CMD_EEPROM) += cmd_eeprom.o
COBJS-$(CONFIG_CMD_ELF) += cmd_elf.o
COBJS-$(CONFIG_SYS_HUSH_PARSER) += cmd_exit.o
COBJS-$(CONFIG_CMD_EXT4) += cmd_ext4.o
COBJS-$(CONFIG_CMD_EXT2) += cmd_ext2.o
COBJS-$(CONFIG_CMD_FAT) += cmd_fat.o
COBJS-$(CONFIG_CMD_FDC)$(CONFIG_CMD_FDOS) += cmd_fdc.o
COBJS-$(CONFIG_OF_LIBFDT) += cmd_fdt.o fdt_support.o
COBJS-$(CONFIG_CMD_FDOS) += cmd_fdos.o
COBJS-$(CONFIG_CMD_FITUPD) += cmd_fitupd.o
COBJS-$(CONFIG_CMD_FLASH) += cmd_flash.o
ifdef CONFIG_FPGA
COBJS-$(CONFIG_CMD_FPGA) += cmd_fpga.o
endif
COBJS-$(CONFIG_CMD_FS_GENERIC) += cmd_fs.o
COBJS-$(CONFIG_CMD_FUSE) += cmd_fuse.o
COBJS-$(CONFIG_CMD_GETTIME) += cmd_gettime.o
COBJS-$(CONFIG_CMD_GPIO) += cmd_gpio.o
COBJS-$(CONFIG_CMD_I2C) += cmd_i2c.o
COBJS-$(CONFIG_CMD_HASH) += cmd_hash.o
COBJS-$(CONFIG_CMD_IDE) += cmd_ide.o
COBJS-$(CONFIG_CMD_IMMAP) += cmd_immap.o
COBJS-$(CONFIG_CMD_INI) += cmd_ini.o
COBJS-$(CONFIG_CMD_IRQ) += cmd_irq.o
COBJS-$(CONFIG_CMD_ITEST) += cmd_itest.o
COBJS-$(CONFIG_CMD_JFFS2) += cmd_jffs2.o
COBJS-$(CONFIG_CMD_CRAMFS) += cmd_cramfs.o
COBJS-$(CONFIG_CMD_LDRINFO) += cmd_ldrinfo.o
COBJS-$(CONFIG_CMD_LED) += cmd_led.o
COBJS-$(CONFIG_CMD_LICENSE) += cmd_license.o
COBJS-$(CONFIG_CMD_LOAD) += cmd_load.o
COBJS-$(CONFIG_LOGBUFFER) += cmd_log.o
COBJS-$(CONFIG_ID_EEPROM) += cmd_mac.o
COBJS-$(CONFIG_CMD_MD5SUM) += cmd_md5sum.o
COBJS-$(CONFIG_CMD_MEMORY) += cmd_mem.o
COBJS-$(CONFIG_CMD_IO) += cmd_io.o
COBJS-$(CONFIG_CMD_MFSL) += cmd_mfsl.o
COBJS-$(CONFIG_MII) += miiphyutil.o
COBJS-$(CONFIG_CMD_MII) += miiphyutil.o
COBJS-$(CONFIG_PHYLIB) += miiphyutil.o
COBJS-$(CONFIG_CMD_MII) += cmd_mii.o
ifdef CONFIG_PHYLIB
COBJS-$(CONFIG_CMD_MII) += cmd_mdio.o
endif
COBJS-$(CONFIG_CMD_MISC) += cmd_misc.o
COBJS-$(CONFIG_CMD_MMC) += cmd_mmc.o
COBJS-$(CONFIG_CMD_MMC_SPI) += cmd_mmc_spi.o
COBJS-$(CONFIG_MP) += cmd_mp.o
COBJS-$(CONFIG_CMD_MTDPARTS) += cmd_mtdparts.o
COBJS-$(CONFIG_CMD_NAND) += cmd_nand.o
COBJS-$(CONFIG_CMD_ZM_NAND) += cmd_nand_zm.o
COBJS-$(CONFIG_CMD_SPINAND) += cmd_spinand.o
COBJS-$(CONFIG_CMD_SFCNAND) += cmd_sfcnand.o
COBJS-$(CONFIG_CMD_NET) += cmd_net.o
COBJS-$(CONFIG_CMD_ONENAND) += cmd_onenand.o
COBJS-$(CONFIG_CMD_OTP) += cmd_otp.o
COBJS-$(CONFIG_CMD_PART) += cmd_part.o
ifdef CONFIG_PCI
COBJS-$(CONFIG_CMD_PCI) += cmd_pci.o
endif
COBJS-y += cmd_pcmcia.o
ifdef CONFIG_MTD_NAND_JZ
COBJS-$(CONFIG_BURNER) += cmd_writespl.o
endif
COBJS-$(CONFIG_CMD_PORTIO) += cmd_portio.o
COBJS-$(CONFIG_CMD_PXE) += cmd_pxe.o
COBJS-$(CONFIG_CMD_READ) += cmd_read.o
COBJS-$(CONFIG_CMD_REGINFO) += cmd_reginfo.o
COBJS-$(CONFIG_CMD_REISER) += cmd_reiser.o
COBJS-$(CONFIG_SANDBOX) += cmd_sandbox.o
COBJS-$(CONFIG_CMD_SATA) += cmd_sata.o
COBJS-$(CONFIG_CMD_SF) += cmd_sf.o
COBJS-$(CONFIG_CMD_SCSI) += cmd_scsi.o
COBJS-$(CONFIG_CMD_SHA1SUM) += cmd_sha1sum.o
COBJS-$(CONFIG_CMD_SETEXPR) += cmd_setexpr.o
COBJS-$(CONFIG_CMD_SOFTSWITCH) += cmd_softswitch.o
ifdef CONFIG_CMD_BURN
COBJS-$(CONFIG_CMD_SPI) += cmd_spi.o
else
COBJS-$(CONFIG_CMD_SPI) += cmd_nor_spi.o
endif
COBJS-$(CONFIG_CMD_SFC_NOR) += cmd_nor_sfc.o
COBJS-$(CONFIG_CMD_SPIBOOTLDR) += cmd_spibootldr.o
COBJS-$(CONFIG_CMD_STRINGS) += cmd_strings.o
COBJS-$(CONFIG_CMD_TERMINAL) += cmd_terminal.o
COBJS-$(CONFIG_CMD_TIME) += cmd_time.o
COBJS-$(CONFIG_CMD_TRACE) += cmd_trace.o
COBJS-$(CONFIG_SYS_HUSH_PARSER) += cmd_test.o
COBJS-$(CONFIG_CMD_TPM) += cmd_tpm.o
COBJS-$(CONFIG_CMD_TSI148) += cmd_tsi148.o
COBJS-$(CONFIG_CMD_UBI) += cmd_ubi.o
COBJS-$(CONFIG_CMD_UBIFS) += cmd_ubifs.o
COBJS-$(CONFIG_CMD_UNIVERSE) += cmd_universe.o
COBJS-$(CONFIG_CMD_UNZIP) += cmd_unzip.o
ifdef CONFIG_CMD_USB
COBJS-y += cmd_usb.o
COBJS-y += usb.o usb_hub.o
COBJS-$(CONFIG_USB_STORAGE) += usb_storage.o
COBJS-$(CONFIG_USB_UVC) += uvc/uvc_driver.o
COBJS-$(CONFIG_USB_UVC) += uvc/uvc_video.o
COBJS-$(CONFIG_USB_LOAD) += usb_load.o
endif
COBJS-$(CONFIG_CMD_USB_MASS_STORAGE) += cmd_usb_mass_storage.o
COBJS-$(CONFIG_CMD_USB_LOAD) += cmd_usb_load.o
COBJS-$(CONFIG_CMD_XIMG) += cmd_ximg.o
COBJS-$(CONFIG_YAFFS2) += cmd_yaffs2.o
COBJS-$(CONFIG_CMD_SPL) += cmd_spl.o
COBJS-$(CONFIG_CMD_ZIP) += cmd_zip.o
COBJS-$(CONFIG_CMD_ZFS) += cmd_zfs.o
COBJS-$(CONFIG_CMD_USBHID) += cmd_hid.o
COBJS-$(CONFIG_DDR_PARAM_DEBUG) += cmd_ddr.o
COBJS-$(CONFIG_DDR_PARAM_DEBUG) += cmd_ddr_param.o
# others
COBJS-$(CONFIG_BOOTSTAGE) += bootstage.o
COBJS-$(CONFIG_CONSOLE_MUX) += iomux.o
COBJS-$(CONFIG_FLASH) += flash.o
COBJS-$(CONFIG_CMD_KGDB) += kgdb.o kgdb_stubs.o
COBJS-$(CONFIG_I2C_EDID) += edid.o
COBJS-$(CONFIG_KALLSYMS) += kallsyms.o
COBJS-y += splash.o
COBJS-$(CONFIG_LCD) += lcd.o
COBJS-$(CONFIG_LYNXKDI) += lynxkdi.o
COBJS-$(CONFIG_MENU) += menu.o
COBJS-$(CONFIG_MODEM_SUPPORT) += modem.o
COBJS-$(CONFIG_UPDATE_TFTP) += update.o
COBJS-$(CONFIG_USB_KEYBOARD) += usb_kbd.o
COBJS-$(CONFIG_CMD_DFU) += cmd_dfu.o
COBJS-$(CONFIG_CMD_GPT) += cmd_gpt.o
COBJS-$(CONFIG_CMD_BURN) += cmd_burn.o
COBJS-$(CONFIG_SOFT_BURNER) += cmd_softburn.o
COBJS-$(CONFIG_SOFT_BURNER_V2) += cmd_softburn.o
COBJS-$(CONFIG_CMD_EFUSE) += cmd_efuse.o
COBJS-$(CONFIG_CMD_BATTERYDET) += cmd_batterydet.o
endif

ifdef CONFIG_SPL_BUILD
COBJS-$(CONFIG_ENV_IS_IN_FLASH) += env_flash.o
COBJS-$(CONFIG_SPL_YMODEM_SUPPORT) += xyzModem.o
COBJS-$(CONFIG_SPL_NET_SUPPORT) += miiphyutil.o
# environment
COBJS-$(CONFIG_SPL_ENV_SUPPORT) += env_attr.o
COBJS-$(CONFIG_SPL_ENV_SUPPORT) += env_flags.o
COBJS-$(CONFIG_SPL_ENV_SUPPORT) += env_callback.o
ifneq ($(CONFIG_SPL_NET_SUPPORT),y)
COBJS-$(CONFIG_ENV_IS_NOWHERE) += env_nowhere.o
COBJS-$(CONFIG_ENV_IS_IN_MMC) += env_mmc.o
COBJS-$(CONFIG_ENV_IS_IN_NAND) += env_nand.o
COBJS-$(CONFIG_ENV_IS_IN_SPI_FLASH) += env_sf.o
COBJS-$(CONFIG_ENV_IS_IN_FLASH) += env_flash.o
else
COBJS-y += env_nowhere.o
endif
endif
# core command
COBJS-y += cmd_nvedit.o
#environment
COBJS-y += env_common.o
#others
ifdef CONFIG_DDR_SPD
SPD := y
endif
ifdef CONFIG_SPD_EEPROM
SPD := y
endif
COBJS-$(SPD) += ddr_spd.o
COBJS-$(CONFIG_HWCONFIG) += hwconfig.o
COBJS-$(CONFIG_BOUNCE_BUFFER) += bouncebuf.o
COBJS-$(CONFIG_CMD_CONSOLE) += console.o
COBJS-y += dlmalloc.o
COBJS-y += image.o
COBJS-$(CONFIG_OF_LIBFDT) += image-fdt.o
COBJS-$(CONFIG_FIT) += image-fit.o
COBJS-$(CONFIG_FIT_SIGNATURE) += image-sig.o
COBJS-y += memsize.o
COBJS-y += stdio.o

COBJS	:= $(sort $(COBJS-y))
XCOBJS	:= $(sort $(XCOBJS-y))
SRCS	:= $(COBJS:.o=.c) $(XCOBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))
XOBJS	:= $(addprefix $(obj),$(XCOBJS))

ifdef CONFIG_CMD_JZ_VPU_TEST
LOCAL_CFLAGS += -I$(TOPDIR)/drivers/video/jz_vpu/ -I$(TOPDIR)/drivers/video/jz_vpu/libh264 -I$(TOPDIR)/drivers/video/jz_vpu/libh264/include
CFLAGS += $(LOCAL_CFLAGS)
CPPFLAGS += $(LOCAL_CFLAGS)
endif

CPPFLAGS += -I..

all:	$(LIB) $(XOBJS)

$(LIB): $(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

$(obj)env_embedded.o: $(src)env_embedded.c $(obj)../tools/envcrc
	$(CC) $(AFLAGS) -Wa,--no-warn \
		-DENV_CRC=$(shell $(obj)../tools/envcrc) \
		-c -o $@ $(src)env_embedded.c

$(obj)../tools/envcrc:
	$(MAKE) -C ../tools

# SEE README.arm-unaligned-accesses
$(obj)hush.o: CFLAGS += $(PLATFORM_NO_UNALIGNED)
$(obj)fdt_support.o: CFLAGS += $(PLATFORM_NO_UNALIGNED)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################

ifeq ($(BR2_PACKAGE_FCS96XFMAC),y)
FCS96XFMAC_VERSION = 1.0.0
FCS96XFMAC_SITE = $(TOPDIR)/sunmi/x2600/kernel/drivers/net/wireless/fcs96xfmac
FCS96XFMAC_SITE_METHOD = local
FCS96XFMAC_INSTALL_TARGET = YES
FCS96XFMAC_SUBDIR = fmac

FCS96XFMAC_MAKE_OPTS = \
	CC="$(TARGET_CC)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	KLIB_BUILD="$(O)/build/linux-custom/" \
	KLIB="$(O)/build/linux-custom/"

FCS96XFMAC_DEPENDENCIES = linux

define FCS96XFMAC_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) $(FCS96XFMAC_MAKE_OPTS) -C $(@D)/$(FCS96XFMAC_SUBDIR) defconfig-brcmfmac
	$(TARGET_MAKE_ENV) $(MAKE) $(FCS96XFMAC_MAKE_OPTS) -C $(@D)/$(FCS96XFMAC_SUBDIR) modules
endef

define FCS96XFMAC_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/usr/lib/modules/brcmfmac
	$(INSTALL) -D -m 0755 $(@D)/fmac/compat/compat.ko $(TARGET_DIR)/usr/lib/modules/brcmfmac/
	$(INSTALL) -D -m 0755 $(@D)/fmac/net/mac80211/mac80211.ko $(TARGET_DIR)/usr/lib/modules/brcmfmac/
	$(INSTALL) -D -m 0755 $(@D)/fmac/drivers/net/wireless/morse/morse.ko $(TARGET_DIR)/usr/lib/modules/brcmfmac/
	$(INSTALL) -D -m 0755 $(@D)/fmac/drivers/net/wireless/morse/dot11ah/dot11ah.ko $(TARGET_DIR)/usr/lib/modules/brcmfmac/
	$(INSTALL) -D -m 0755 $(@D)/fmac/drivers/net/wireless/broadcom/brcm80211/brcmfmac/brcmfmac.ko $(TARGET_DIR)/usr/lib/modules/brcmfmac/
	$(INSTALL) -D -m 0755 $(@D)/fmac/drivers/net/wireless/broadcom/brcm80211/brcmutil/brcmutil.ko $(TARGET_DIR)/usr/lib/modules/brcmfmac/
	$(INSTALL) -D -m 0755 $(@D)/fmac/net/wireless/cfg80211.ko $(TARGET_DIR)/usr/lib/modules/brcmfmac/
endef

$(eval $(generic-package))
endif

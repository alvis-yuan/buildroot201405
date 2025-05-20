################################################################################
#
# wl
#
################################################################################

WL_FMAC_MIPS32_STATIC_VERSION = 1118
WL_FMAC_MIPS32_STATIC_SITE = /dl/
WL_FMAC_MIPS32_STATIC_PKG_DIR = package/wl_fmac_mips32_static

define WL_FMAC_MIPS32_STATIC_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 -D $(@D)/wl_tool_NL80211 $(TARGET_DIR)/usr/bin/wl
endef

$(eval $(generic-package))

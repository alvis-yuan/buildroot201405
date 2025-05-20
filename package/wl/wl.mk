################################################################################
#
# wl
#
################################################################################

WL_VERSION = 0704
WL_SITE = /dl/
WL_PKG_DIR = package/wl

define WL_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 -D $(@D)/wl $(TARGET_DIR)/usr/bin/wl
endef

$(eval $(generic-package))

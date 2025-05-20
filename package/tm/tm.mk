################################################################################
#
# makedevs
#
################################################################################

# source included in buildroot
TM_VERSION = 1.0.0
TM_SITE = $(TOPDIR)/sunmi/x1021/tm
TM_SITE_METHOD = local
TM_LICENSE = LGPLv2.1
TM_LICENSE_FILES = COPYING
TM_INSTALL_STAGING = YES

define TM_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS)" \
		-C $(@D) all
endef

define TM_INSTALL_STAGING_CMDS
	$(INSTALL) -D -m 0755 $(@D)/tm $(STAGING_DIR)/usr/sbin
	ln -fs /usr/sbin/tm $(TARGET_DIR)/usr/sbin/dump_hwinfo
endef

define TM_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/tm $(TARGET_DIR)/usr/sbin
endef

$(eval $(generic-package))

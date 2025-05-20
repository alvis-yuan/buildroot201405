################################################################################
#
# makedevs
#
################################################################################

# source included in buildroot
BT_SERVICE_VERSION = 1.0.0
BT_SERVICE_SITE = $(TOPDIR)/sunmi/x1021/bt_service
BT_SERVICE_SITE_METHOD = local
BT_SERVICE_LICENSE = LGPLv2.1
BT_SERVICE_LICENSE_FILES = COPYING
BT_SERVICE_INSTALL_STAGING = YES

define BT_SERVICE_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS)" \
		-C $(@D) all
endef

define BT_SERVICE_INSTALL_STAGING_CMDS
	$(INSTALL) -D -m 0755 $(@D)/bt_service $(STAGING_DIR)/usr/sbin
endef

define BT_SERVICE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/bt_service $(TARGET_DIR)/usr/sbin
endef

$(eval $(generic-package))

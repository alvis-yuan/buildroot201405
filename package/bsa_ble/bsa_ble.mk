################################################################################
#
# makedevs
#
################################################################################

# source included in buildroot
BSA_BLE_VERSION = 1.0.0
BSA_BLE_SITE = $(TOPDIR)/sunmi/x1021/bsa_ble
BSA_BLE_SITE_METHOD = local
BSA_BLE_LICENSE = LGPLv2.1
BSA_BLE_LICENSE_FILES = COPYING
BSA_BLE_INSTALL_STAGING = YES

define BSA_BLE_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS)" \
		-C $(@D) all
endef

define BSA_BLE_INSTALL_STAGING_CMDS
	$(INSTALL) -D -m 0755 $(@D)/bsa_ble_app $(STAGING_DIR)/usr/sbin
endef

define BSA_BLE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/bsa_ble_app $(TARGET_DIR)/usr/sbin
endef

$(eval $(generic-package))

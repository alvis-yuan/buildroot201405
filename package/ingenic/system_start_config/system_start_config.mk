################################################################################
#
# system_start_config
#
################################################################################

SYSTEM_START_CONFIG_VERSION = 1.0
SYSTEM_START_CONFIG_SITE_METHOD = local
SYSTEM_START_CONFIG_SITE = ingenic/system_start_config

ifeq ($(SYSTEM_START_CONFIG_ADB_SERVICE),y)
SYSTEM_START_CONFIG_ADB_SERVICE_BASH = $(INSTALL) -D -m 755 $(@D)/S90adb $(TARGET_DIR)/etc/init.d
endif

define SYSTEM_START_CONFIG_INSTALL_TARGET_CMDS
	$(SYSTEM_START_CONFIG_ADB_SERVICE_BASH)
endef

$(eval $(generic-package))

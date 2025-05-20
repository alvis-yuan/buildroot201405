################################################################################
#
# bcm55512_firmware
#
################################################################################

BCM55512_FIRMWARE_VERSION = 1.0
BCM55512_FIRMWARE_SITE_METHOD = local
BCM55512_FIRMWARE_SITE = ingenic/x2600/bcm55512_firmware

ifeq ($(BCM55512_FIRMWARE_BT_PROTOCOL_BTSTACK),y)
BCM55512_FIRMWARE_MKDIR = mkdir -p $(TARGET_DIR)$(shell echo $(BCM55512_FIRMWARE_FILE_PATH)) $(TARGET_DIR)/data/bluetooth/
endif

define BCM55512_FIRMWARE_INSTALL_TARGET_CMDS
	$(BCM55512_FIRMWARE_MKDIR)
	$(INSTALL) -D -m 644 $(@D)/firmware/* $(TARGET_DIR)$(shell echo $(BCM55512_FIRMWARE_FILE_PATH))
	$(BCM55512_FIRMWARE_BT_INSTALL)
endef

$(eval $(generic-package))

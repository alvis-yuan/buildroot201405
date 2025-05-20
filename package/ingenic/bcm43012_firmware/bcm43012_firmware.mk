################################################################################
#
# bcm43012_firmware
#
################################################################################

BCM43012_FIRMWARE_VERSION = 1.0
BCM43012_FIRMWARE_SITE_METHOD = local
BCM43012_FIRMWARE_SITE = ingenic/x2600/bcm43012_firmware

define BCM43012_FIRMWARE_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)$(shell echo $(BCM43012_FIRMWARE_FILE_PATH))
	cp -r  $(@D)/firmware/* $(TARGET_DIR)$(shell echo $(BCM43012_FIRMWARE_FILE_PATH))
endef

$(eval $(generic-package))

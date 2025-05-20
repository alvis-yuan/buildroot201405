################################################################################
#
# morse6108_firmware
#
################################################################################

MORSE6108_FIRMWARE_VERSION = 1.0
MORSE6108_FIRMWARE_SITE_METHOD = local
MORSE6108_FIRMWARE_SITE = ingenic/x2600/morse6108_firmware

define MORSE6108_FIRMWARE_INSTALL_TARGET_CMDS
	cp -r  $(@D)/firmware/* $(TARGET_DIR)$(shell echo $(MORSE6108_FIRMWARE_FILE_PATH))
endef

$(eval $(generic-package))

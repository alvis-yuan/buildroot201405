################################################################################
#
# fcs96xfmac_firmware
#
################################################################################

FCS96XFMAC_FIRMWARE_VERSION = 1.0
FCS96XFMAC_FIRMWARE_SITE_METHOD = local
FCS96XFMAC_FIRMWARE_SITE = ingenic/x2600/fcs96xfmac_firmware

define FCS96XFMAC_FIRMWARE_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)$(shell echo $(FCS96XFMAC_FIRMWARE_FILE_PATH))
	cp -r  $(@D)/firmware/* $(TARGET_DIR)$(shell echo $(FCS96XFMAC_FIRMWARE_FILE_PATH))
endef

$(eval $(generic-package))
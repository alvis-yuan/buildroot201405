################################################################################
#
# bcm43438_firmware
#
################################################################################

BCM43438_FIRMWARE_VERSION = 1.0
BCM43438_FIRMWARE_SITE_METHOD = local
#BCM43438_FIRMWARE_SITE = ingenic/bcm43438_firmware
ifeq ($(findstring x1600, $(BR2_LINUX_KERNEL_CUSTOM_LOCAL_PATH)) , x1600)
	BCM43438_FIRMWARE_SITE = ingenic/x1600/bcm43438_firmware
else
ifeq ($(findstring x2600, $(BR2_LINUX_KERNEL_CUSTOM_LOCAL_PATH)) , x2600)
	BCM43438_FIRMWARE_SITE = ingenic/x2600/bcm43438_firmware
else
	BCM43438_FIRMWARE_SITE = ingenic/x1021/bcm43438_firmware
endif
endif

ifeq ($(BCM43438_FIRMWARE_BT_PROTOCOL_BLUEZ),y)
BCM43438_FIRMWARE_MKDIR = mkdir -p $(TARGET_DIR)$(shell echo $(BCM43438_FIRMWARE_FILE_PATH))
BCM43438_FIRMWARE_BT_ENABLE_FILE = bt_enable_bluez
BCM43438_FIRMWARE_BT_DISABLE_FILE = bt_disable_bluez
BCM43438_FIRMWARE_BT_INSTALL = $(INSTALL) -D -m 755 $(@D)/bin/brcm_patchram_plus $(@D)/bin/bt_enable $(@D)/bin/wl $(@D)/bin/bt_disable $(TARGET_DIR)/usr/bin/
endif

ifeq ($(BCM43438_FIRMWARE_BT_PROTOCOL_BSA),y)
BCM43438_FIRMWARE_MKDIR = mkdir -p $(TARGET_DIR)$(shell echo $(BCM43438_FIRMWARE_FILE_PATH)) $(TARGET_DIR)/data/bluetooth/
BCM43438_FIRMWARE_BT_ENABLE_FILE = bt_enable_bsa
BCM43438_FIRMWARE_BT_DISABLE_FILE = bt_disable_bsa
BCM43438_FIRMWARE_BT_INSTALL = $(INSTALL) -D -m 755 $(@D)/bin/bsa_server $(@D)/bin/bt_enable $(@D)/bin/wl $(@D)/bin/bt_disable $(TARGET_DIR)/usr/bin/
endif

define BCM43438_FIRMWARE_INSTALL_TARGET_CMDS
	$(BCM43438_FIRMWARE_MKDIR)
	$(INSTALL) -D -m 644 $(@D)/firmware/* $(TARGET_DIR)$(shell echo $(BCM43438_FIRMWARE_FILE_PATH))
	cp $(@D)/bin/$(BCM43438_FIRMWARE_BT_ENABLE_FILE) $(@D)/bin/bt_enable
	cp $(@D)/bin/$(BCM43438_FIRMWARE_BT_DISABLE_FILE) $(@D)/bin/bt_disable
	sed -i "s/FIRMWARE_FILE_PATH/$(shell echo $(BCM43438_FIRMWARE_FILE_PATH) | sed "s/\\//\\\\\//g")/g" $(@D)/bin/bt_enable
	sed -i "s/FIRMWARE_BT_UART_DEV_PATH/$(shell echo $(BCM43438_FIRMWARE_BT_UART_DEV_PATH) | sed "s/\\//\\\\\//g")/g" $(@D)/bin/bt_enable
	$(BCM43438_FIRMWARE_BT_INSTALL)
endef

$(eval $(generic-package))

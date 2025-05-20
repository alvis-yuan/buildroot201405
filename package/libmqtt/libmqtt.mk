################################################################################
#
# makedevs
#
################################################################################

# source included in buildroot
LIBMQTT_VERSION = 1.0.0
LIBMQTT_SITE = $(TOPDIR)/sunmi/libmqtt
LIBMQTT_SITE_METHOD = local
LIBMQTT_LICENSE = LGPLv2.1
LIBMQTT_LICENSE_FILES = COPYING
LIBMQTT_INSTALL_STAGING = YES

define LIBMQTT_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS)" \
		-C $(@D) all
endef

define LIBMQTT_INSTALL_STAGING_CMDS
	$(INSTALL) -D -m 0755 $(@D)/libmqtt.so $(STAGING_DIR)/usr/lib
    $(INSTALL) -D -m 0755 $(@D)/inc/mqtt.h $(STAGING_DIR)/usr/include
    $(INSTALL) -D -m 0755 $(@D)/inc/mqtt_pal.h $(STAGING_DIR)/usr/include
    $(INSTALL) -D -m 0755 $(@D)/inc/mqtt_os.h $(STAGING_DIR)/usr/include
endef

define LIBMQTT_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/libmqtt.so $(TARGET_DIR)/usr/lib
endef

$(eval $(generic-package))

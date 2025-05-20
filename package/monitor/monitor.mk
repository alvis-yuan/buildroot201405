################################################################################
#
# makedevs
#
################################################################################

# source included in buildroot
MONITOR_VERSION = 1.00
MONITOR_SITE = $(TOPDIR)/sunmi/monitor
MONITOR_SITE_METHOD = local
MONITOR_LICENSE = LGPLv2.1
MONITOR_LICENSE_FILES = COPYING

define MONITOR_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS)" \
		-C $(@D) all
endef


define MONITOR_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/monitor $(TARGET_DIR)/usr/sbin
	ln -fs /usr/sbin/monitor $(TARGET_DIR)/usr/sbin/logcat
	$(INSTALL) -D -m 0755 $(@D)/ini_get $(TARGET_DIR)/usr/sbin
	ln -fs /usr/sbin/ini_get $(TARGET_DIR)/usr/sbin/ini_set
endef

$(eval $(generic-package))

################################################################################
#
# makedevs
#
################################################################################

# source included in buildroot
LIBBASE_VERSION = 1.0.0
LIBBASE_SITE = $(TOPDIR)/sunmi/libbase
LIBBASE_SITE_METHOD = local
LIBBASE_LICENSE = LGPLv2.1
LIBBASE_LICENSE_FILES = COPYING
LIBBASE_INSTALL_STAGING = YES


define LIBBASE_BUILD_CMDS
#	$(MAKE) LIBBASE_VER=$(BR2_LIBBASE_VER) CC="$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS)"  \
#		-C $(@D) all
endef

define LIBBASE_INSTALL_STAGING_CMDS
	cp -dRf $(LIBBASE_SITE)/lib/mips/libbase.so* $(STAGING_DIR)/usr/lib
    $(INSTALL) -D -m 0755 $(LIBBASE_SITE)/inc/base.h $(STAGING_DIR)/usr/include
endef

define LIBBASE_INSTALL_TARGET_CMDS
	cp -dRf $(LIBBASE_SITE)/lib/mips/libbase.so* $(TARGET_DIR)/usr/lib
endef

$(eval $(generic-package))

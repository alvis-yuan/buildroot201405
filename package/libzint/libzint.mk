################################################################################
#
#zint
#
################################################################################

# source included in buildroot
LIBZINT_NAME = zint
LIBZINT_VERSION = 2.6.3
LIBZINT_SOURCE =  zint-${LIBZINT_VERSION}.src.tar.gz
LIBZINT_SITE =  http://downloads.sourceforge.net/project/zint/zint/${LIBZINT_VERSION}
#LIBZINT_SITE_METHOD = http
LIBZINT_LICENSE = GPLv3+
LIBZINT_LICENSE_FILES = COPYING
LIBZINT_INSTALL_STAGING = YES
LIBZINT_DEPENDENCIES = zlib libpng
LIBZINT_CONF_OPT = -DCMAKE_INSTALL_PREFIX:FILEPATH=$(STAGING_DIR)/usr

define LIBZINT_BUILD_CMDS
#	$(MAKE) LIBZINT_VER=$(BR2_LIBZINT_VER) CC="$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS)"  \
#		-C $(@D) all
	$(MAKE) -C $(@D)
endef

define LIBZINT_INSTALL_STAGING_CMDS
	cp -dRf $(@D)/backend/libzint.so* $(STAGING_DIR)/usr/lib
    $(INSTALL) -D -m 0755 $(@D)/backend/zint.h  $(STAGING_DIR)/usr/include
endef

define LIBZINT_INSTALL_TARGET_CMDS
	cp -dRf $(@D)/backend/libzint.so* $(TARGET_DIR)/usr/lib
	cp -dRf	$(@D)/frontend/zint     $(TARGET_DIR)/usr/bin
endef

$(eval $(cmake-package))

################################################################################
#
# libnl
#
################################################################################

LIBNL_VERSION = 3.5.0
LIBNL_SITE = https://github.com/thom311/libnl/releases/download/libnl$(subst .,_,$(LIBNL_VERSION))
LIBNL_LICENSE = LGPL-2.1+
LIBNL_LICENSE_FILES = COPYING
LIBNL_INSTALL_STAGING = YES
LIBNL_DEPENDENCIES = host-bison host-flex host-pkgconf

ifeq ($(BR2_PACKAGE_LIBNL_TOOLS),y)
LIBNL_CONF_OPTS += --enable-cli
else
LIBNL_CONF_OPTS += --disable-cli
endif

ifeq ($(BR2_PACKAGE_CHECK),y)
LIBNL_DEPENDENCIES += check
LIBNL_CONF_OPTS += --enable-unit-tests
else
LIBNL_CONF_OPTS += --disable-unit-tests
endif

define LIBNL_COPY_FILE
    cp $(@D)/include/linux-private/linux/if_macsec.h $(STAGING_DIR)/usr/include/linux
endef
LIBNL_POST_INSTALL_TARGET_HOOKS += LIBNL_COPY_FILE

$(eval $(autotools-package))


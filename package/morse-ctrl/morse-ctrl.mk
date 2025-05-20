################################################################################
#
# morse-ctrl
#
################################################################################

MORSE_CTRL_VERSION = rel_1_11_3_2024_Mar_28
MORSE_CTRL_SITE = /dl/
MORSE_CTRL_LICENSE = Proprietary
MORSE_CTRL_REDISTRIBUTE = NO
MORSE_CTRL_GIT_SRC_DIR = $(if $(MORSE_CTRL_OVERRIDE_SRCDIR),$(MORSE_CTRL_OVERRIDE_SRCDIR),$(MORSE_CTRL_DL_DIR)/git)
MORSE_CTRL_GIT_DESCRIBE = $(shell git -C $(MORSE_CTRL_GIT_SRC_DIR) describe --always --dirty)
MORSE_CTRL_CFLAGS = $(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include/libnl3/
MORSE_CTRL_PKG_DIR = package/morse-ctrl

RSYNC = rsync -aHAX

# Sanitise source code (for releases)
define MORSE_CTRL_SANITISE_SOURCE
	cd $(MORSE_CTRL_GIT_SRC_DIR) && \
	tools/make_code_release.py tools/code_release_config.toml && \
	rm -rf $(@D)/* && \
	unzip $(MORSE_CTRL_GIT_SRC_DIR)/morsectrl_$(MORSE_CTRL_GIT_DESCRIBE).zip && \
	$(RSYNC) $(MORSE_CTRL_GIT_SRC_DIR)/morsectrl_$(MORSE_CTRL_GIT_DESCRIBE)/* $(@D)/
endef

ifeq ($(BR2_PACKAGE_MORSE_CTRL_TRANS_NL80211),y)
MORSE_CTRL_DEPENDENCIES += libnl
MORSE_CTRL_MAKE_CFG += CONFIG_MORSE_TRANS_NL80211=1
MORSE_CTRL_CFLAGS += -I$(STAGING_DIR)/usr/include/libnl3/
endif

ifeq ($(BR2_PACKAGE_MORSE_CTRL_TRANS_FTDI_SPI),y)
MORSE_CTRL_MAKE_CFG += CONFIG_MORSE_TRANS_FTDI_SPI=1
endif

define MORSE_CTRL_BUILD_CMDS
	$(MAKE) $(MORSE_CTRL_MAKE_CFG) \
		MORSECTRL_VERSION_STRING="$(MORSE_CTRL_GIT_DESCRIBE)" \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(MORSE_CTRL_CFLAGS)" \
		-C $(@D)
endef

define MORSE_CTRL_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 -D $(@D)/morsectrl $(TARGET_DIR)/usr/bin/morsectrl
	$(INSTALL) -m 0755 -D $(@D)/morse_cli $(TARGET_DIR)/usr/bin/morse_cli
endef

ifeq ($(BR2_PACKAGE_MORSE_CTRL_SANITISE),y)
MORSE_CTRL_PRE_BUILD_HOOKS += MORSE_CTRL_SANITISE_SOURCE
endif

$(eval $(generic-package))

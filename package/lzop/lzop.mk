################################################################################
#
# lzop
#
################################################################################

LZOP_VERSION = 1.04
LZOP_SITE = http://www.lzop.org/download/
LZOP_LICENSE = GPLv2+
LZOP_LICENSE_FILES = COPYING
LZOP_DEPENDENCIES = lzo
LZOP_LIBTOOL_PATCH = NO

$(eval $(autotools-package))
$(eval $(host-autotools-package))

LZOP = $(HOST_DIR)/usr/bin/lzop

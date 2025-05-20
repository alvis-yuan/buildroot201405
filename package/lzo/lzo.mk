################################################################################
#
# lzo
#
################################################################################

LZO_VERSION = 2.10
LZO_SITE = http://www.oberhumer.com/opensource/lzo/download
LZO_LICENSE = GPLv2+
LZO_LICENSE_FILES = COPYING
LZO_INSTALL_STAGING = YES
LZO_LIBTOOL_PATCH = NO

$(eval $(autotools-package))
$(eval $(host-autotools-package))

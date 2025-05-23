################################################################################
#
# libopenssl
#
################################################################################

LIBOPENSSL_VERSION = 1.1.1t
LIBOPENSSL_SITE = https://www.openssl.org/source
LIBOPENSSL_SOURCE = openssl-$(LIBOPENSSL_VERSION).tar.gz
LIBOPENSSL_LICENSE = OpenSSL or SSLeay
LIBOPENSSL_LICENSE_FILES = LICENSE
LIBOPENSSL_INSTALL_STAGING = YES
LIBOPENSSL_DEPENDENCIES = zlib
HOST_LIBOPENSSL_DEPENDENCIES = host-zlib
LIBOPENSSL_TARGET_ARCH = $(call qstrip,$(BR2_PACKAGE_LIBOPENSSL_TARGET_ARCH))
LIBOPENSSL_CFLAGS = $(TARGET_CFLAGS)
#LIBOPENSSL_PROVIDES = openssl
#LIBOPENSSL_CPE_ID_VENDOR = $(LIBOPENSSL_PROVIDES)
#LIBOPENSSL_CPE_ID_PRODUCT = $(LIBOPENSSL_PROVIDES)

ifeq ($(BR2_m68k_cf),y)
# relocation truncated to fit: R_68K_GOT16O
LIBOPENSSL_CFLAGS += -mxgot
# resolves an assembler "out of range error" with blake2 and sha512 algorithms
LIBOPENSSL_CFLAGS += -DOPENSSL_SMALL_FOOTPRINT
endif

ifeq ($(BR2_USE_MMU),)
LIBOPENSSL_CFLAGS += -DHAVE_FORK=0 -DOPENSSL_NO_MADVISE
endif

ifeq ($(BR2_PACKAGE_HAS_CRYPTODEV),y)
LIBOPENSSL_DEPENDENCIES += cryptodev
endif


ifeq ($(BR2_TOOLCHAIN_USES_MUSL),y)
LIBOPENSSL_CFLAGS += -DOPENSSL_NO_ASYNC
endif
ifeq ($(BR2_TOOLCHAIN_HAS_UCONTEXT),)
LIBOPENSSL_CFLAGS += -DOPENSSL_NO_ASYNC
endif

define HOST_LIBOPENSSL_CONFIGURE_CMDS
	cd $(@D); \
		$(HOST_CONFIGURE_OPTS) \
		./config \
		--prefix=$(HOST_DIR) \
		--openssldir=$(HOST_DIR)/etc/ssl \
		no-tests \
		no-fuzz-libfuzzer \
		no-fuzz-afl \
		shared \
		zlib-dynamic
endef

define LIBOPENSSL_CONFIGURE_CMDS
	cd $(@D); \
		$(TARGET_CONFIGURE_ARGS) \
		$(TARGET_CONFIGURE_OPTS) \
		 CFLAGS="$(LIBOPENSSL_CFLAGS)" \
		./Configure \
			$(LIBOPENSSL_TARGET_ARCH) \
			--prefix=/usr \
			--openssldir=/etc/ssl \
			$(if $(BR2_TOOLCHAIN_HAS_LIBATOMIC),-latomic) \
			$(if $(BR2_TOOLCHAIN_HAS_THREADS),threads,no-threads) \
			$(if $(BR2_STATIC_LIBS),no-shared,shared) \
			$(if $(BR2_PACKAGE_HAS_CRYPTODEV),enable-devcryptoeng) \
			no-rc5 \
			enable-camellia \
			no-tests \
			no-fuzz-libfuzzer \
			no-fuzz-afl \
			no-afalgeng \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_CHACHA),,no-chacha) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_RC2),,no-rc2) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_RC4),,no-rc4) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_MD2),,no-md2) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_MD4),,no-md4) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_MDC2),,no-mdc2) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_BLAKE2),,no-blake2) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_IDEA),,no-idea) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_SEED),,no-seed) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_DES),,no-des) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_RMD160),,no-rmd160) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_WHIRLPOOL),,no-whirlpool) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_BLOWFISH),,no-bf) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_SSL),,no-ssl) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_SSL2),,no-ssl2) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_SSL3),,no-ssl3) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_WEAK_SSL),,no-weak-ssl-ciphers) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_PSK),,no-psk) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_CAST),,no-cast) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_UNSECURE),,no-unit-test no-crypto-mdebug-backtrace no-crypto-mdebug no-autoerrinit) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_DYNAMIC_ENGINE),,no-dynamic-engine ) \
			$(if $(BR2_PACKAGE_LIBOPENSSL_ENABLE_COMP),,no-comp) \
			$(if $(BR2_STATIC_LIBS),zlib,zlib-dynamic) \
			$(if $(BR2_STATIC_LIBS),no-dso)
endef

# libdl is not available in a static build, and this is not implied by no-dso
ifeq ($(BR2_STATIC_LIBS),y)
define LIBOPENSSL_FIXUP_STATIC_MAKEFILE
	$(SED) 's#-ldl##g' $(@D)/Makefile
endef
LIBOPENSSL_POST_CONFIGURE_HOOKS += LIBOPENSSL_FIXUP_STATIC_MAKEFILE
endif

define HOST_LIBOPENSSL_BUILD_CMDS
	$(HOST_MAKE_ENV) $(MAKE) -C $(@D)
endef

define LIBOPENSSL_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)
endef

define LIBOPENSSL_INSTALL_STAGING_CMDS
	#$(TARGET_MAKE_ENV) $(MAKE) -C $(@D) DESTDIR=$(STAGING_DIR) install
	mkdir -p $(STAGING_DIR)/usr/static/libopenssl/
	mkdir -p $(STAGING_DIR)/usr/static/libopenssl/apps/
	mkdir -p $(STAGING_DIR)/usr/include/libopenssl/
    cp -dRf $(@D)/*.a $(STAGING_DIR)/usr/static/libopenssl/
	cp -dRf $(@D)/apps/*.a $(STAGING_DIR)/usr/static/libopenssl/
	cp -dRf $(@D)/include/openssl/ $(STAGING_DIR)/usr/include/libopenssl/
endef

$(eval $(generic-package))
$(eval $(host-generic-package))

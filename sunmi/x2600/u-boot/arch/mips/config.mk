#
# (C) Copyright 2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

CROSS_COMPILE ?= mips-linux-gnu-

# Handle special prefix in ELDK 4.0 toolchain
ifneq (,$(findstring 4KCle,$(CROSS_COMPILE)))
ENDIANNESS := -EL
endif

ifdef CONFIG_SYS_LITTLE_ENDIAN
ENDIANNESS := -EL
endif

ifdef CONFIG_SYS_BIG_ENDIAN
ENDIANNESS := -EB
endif

# Default to EB if no endianess is configured
ENDIANNESS ?= -EL

PLATFORM_CPPFLAGS += -DCONFIG_MIPS -D__MIPS__

#
# From Linux arch/mips/Makefile
#
# GCC uses -G 0 -mabicalls -fpic as default.  We don't want PIC in the kernel
# code since it only slows down the whole thing.  At some point we might make
# use of global pointer optimizations but their use of $28 conflicts with
# the current pointer optimization.
#
# The DECStation requires an ECOFF kernel for remote booting, other MIPS
# machines may also.  Since BFD is incredibly buggy with respect to
# crossformat linking we rely on the elf2ecoff tool for format conversion.
#
# cflags-y			+= -G 0 -mno-abicalls -fno-pic -pipe
# cflags-y			+= -msoft-float
# LDFLAGS_vmlinux		+= -G 0 -static -n -nostdlib
# MODFLAGS			+= -mlong-calls
#
# On the other hand, we want PIC in the U-Boot code to relocate it from ROM
# to RAM, unless we're building SPL which doesn't relocate. $28 is always
# used as gp.
#
PLATFORM_CPPFLAGS		+= -G 0 $(ENDIANNESS)
PLATFORM_CPPFLAGS		+= -msoft-float -std=gnu89
PLATFORM_LDFLAGS		+= -G 0 -static -n -nostdlib $(ENDIANNESS)
PLATFORM_RELFLAGS		+= -ffunction-sections -fdata-sections
LDFLAGS_FINAL			+= --gc-sections
OBJCFLAGS			+= --remove-section=.dynsym
ifdef CONFIG_SPL_BUILD
PLATFORM_CPPFLAGS		+= -fno-pic -mno-abicalls
else
PLATFORM_CPPFLAGS		+= -fpic -mabicalls
LDFLAGS_FINAL			+= -pie
endif

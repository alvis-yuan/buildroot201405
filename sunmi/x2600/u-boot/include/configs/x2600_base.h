#ifndef __X2600_BASE_H__
#define	__X2600_BASE_H__

#define CONFIG_ROOTFS_SQUASHFS
#define CONFIG_ROOTFS2_SQUASHFS
/* #define CONFIG_ARG_QUIET */
#define CONFIG_SPL_SERIAL_SUPPORT

#define CONFIG_SUNMI_HWINFO
#define CONFIG_BUILD_UBOOT
/*reserved memory 128k for hwinfo*/
#define CONFIG_HWINFO_RESERVED_MEMORY               (32 * 1024)
#define CONFIG_HWINFO_RESERVED_MEMORY_START_PA      (0x7FF8000)

#include "x2600_base_common.h"

#endif /* __X2600_BASE_H__ */

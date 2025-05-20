#ifndef __BC3_HWINFO__
#define __BC3_HWINFO__

/*
 * hwinfo分区概览:
 * 总大小:32768byte (32k),共分成256个存储单元, 每个单元共128个字节, 可存储100个字节数据.
 * 存储单元的格式如下:
 * mask:存储单元的最前面10个字节和最后面10个字节为完整性校验码,写的时候随机产生,只有两个校验码相同时,才确认写成功;
 * size:存储单元中保存的内容的大小;
 * content:存储单元中保存的内容,最大能存储100个字节.
 *
 * 0 mask1 10 szie 18        content    118 mask2 127
 * |-------|------|-----------------------|---------|
 *
 *project name: 0-127
 *sn:
 *pn:
 *
 *
 *large block:
 *
 * 0        mask1  10  szie  18        content        end-10   mask2    end
 * |-----------------|---------|--------------------------|----------------|
 *start                                                                 end
 */

#define CFG_BATTERY_CAPACITY		"BAT_CAP"
#define CFG_WNET_MD					"WNET_MD"

#define HWINFO1_SEEK_START			0/*total 1MB, hwinfo1 start from 0*/
#define HWINFO1_SEEK_STEP			0x20000 /*128KB*/
#define HWINFO1_SEEK_MAX			0x80000/*512KB*/
#define HWINFO2_SEEK_START			0x80000/*total 1MB, hwinfo1 start from 512KB*/
#define HWINFO2_SEEK_STEP			0x20000 /*128KB*/
#define HWINFO2_SEEK_MAX			0x100000/*1024KB*/

#if defined(CONFIG_BUILD_UBOOT) || defined(CONFIG_BUILD_KERNEL)
#define MEMORY_PA_START_ADDR		0x00000000	/*memory start address*/
#define MEMORY_PA_ADDR_LEN			0x8000000	/*memory size, 128MB*/
#define HWINFO_VA_MEMORY_ADDR		(0x7ff8000 | 0xa0000000)    /*va top memory 32KB, total 128M*/
#define HWINFO_VA_MEMORY_SIZE		0x8000		/*32KB*/
#define HWINFO_FLASH_ADDR			0x100000	/*1MB seek*/
#define HWINFO_FLASH_TOTAL_SIZE		0x100000	/*1MB*/
#define HWINFO_FLASH_DATA_SIZE		0x8000		/*32KB*/
#endif

enum BATTERY_CFG {
	BATTERY_CFG_EMPTY		= 0x00,
	BATTERY_CFG_ULT1000		= 0x01,
	BATTERY_CFG_BK18650		= 0x02,
	BATTERY_CFG_MAX			= 0xff,
};

enum WNET_CFG {
	WNET_CFG_EMPTY			= 0x00,
	WNET_CFG_G510			= 0x01,
	WNET_CFG_MAX			= 0xff,
};

enum HWINFO_SEEK {
	HWINFO_MASK1_SEEK				= 0,
	HWINFO_SIZE_SEEK				= 10,
	HWINFO_CONTENT_SEEK			= 18,
	HWINFO_MASK2_SEEK				= 118,
	HWINFO_MAX_SEEK				= 127
};

enum HWINFO_LENS {
	HWINFO_MASK1_LENS				= 10,
	HWINFO_SIZE_LENS				= 8,
	HWINFO_CONTENT_LENS			= 100,
	HWINFO_MASK2_LENS				= 10,
	HWINFO_BLOCK_LENS				= 128
};

enum HWINFO_BLOCK {
    HWINFO_BLOCK_PROJECT 				= 0,
    HWINFO_BLOCK_SN						= 1,
    HWINFO_BLOCK_PN						= 2,
	HWINFO_BLOCK_MAC					= 3,
	HWINFO_BLOCK_MQTT_PWD				= 4,
	HWINFO_BLOCK_MQTT_ADDR				= 5,
	HWINFO_BLOCK_MQTT_PORT				= 6,
	HWINFO_BLOCK_LARGE_PEM_START		= 7, /*size:5120 - 8 - 10 - 10*/
	HWINFO_BLOCK_LARGE_PEM_END			= 46,
	HWINFO_BLOCK_LARGE_CFG_START		= 47, /*size:5120 - 8 - 10 - 10*/
	HWINFO_BLOCK_LARGE_CFG_END			= 86,
	HWINFO_BLOCK_BATTERT_CFG			= 87,
	HWINFO_BLOCK_BOOT_SELECT			= 88,/*ping pang boot, null or 0 = default first part boot, 1= second part boot*/
	HWINFO_BLOCK_UPDATE_COUNT			= 89,/*default 0, write once , add once*/

	HWINFO_BLOCK_CRC					= 255,/*crc verify data of the hwinfo block*/
    HWINFO_BLOCK_MAX 					= 256
};

enum HWINFO_OPS {
	HWINFO_SN_WRITE					= 0,
	HWINFO_SN_READ,
	HWINFO_PN_WRITE,
	HWINFO_PN_READ,
	HWINFO_MAC_WRITE,
	HWINFO_MAC_READ,
	HWINFO_PROJECT_READ,
	HWINFO_PROJECT_WRITE,
	HWINFO_MQTT_PWD_WRITE,
	HWINFO_MQTT_PWD_READ,
	HWINFO_MQTT_ADDR_WRITE,
	HWINFO_MQTT_ADDR_READ,
	HWINFO_MQTT_PORT_WRITE,
	HWINFO_MQTT_PORT_READ,
	HWINFO_BASE_INFO_READ, /*相当于sysGetParam("HWINFO") + 获取应用版本号*/
	HWINFO_OPS_MAX,
};

enum FILE_OPS {
	FILE_OPS_G510_UPDATE		= 0,
	FILE_OPS_APP_UPDATE,
	FILE_OPS_FW_UPDATE,
	FILE_OPS_PEM_DOWNLOAD,
	FILE_OPS_CFG_DOWNLOAD,
	FILE_OPS_MAX,
};

enum SYS_OPS {/*ops = operates*/
	SYS_OPS_REBOOT0		= 0, /*正常重启*/
	SYS_OPS_REBOOT1,		/*重启到下载模式*/
	SYS_OPS_POWEROFF,		/*关机*/
	SYS_OPS_FMT_DATA,		/*格式化data分区*/
	SYS_OPS_FMT_ROOT,		/*格式化root分区*/
	SYS_OPS_FMT_KERNEL,		/*格式化kernel分区*/
	SYS_OPS_FMT_HWINFO,		/*格式化hwinfo分区*/
	SYS_OPS_FMT_UBOOT,		/*格式化uboot分区*/
	SYS_OPS_RM_APP0,		/*删除APP0*/
	SYS_OPS_RESTORE_FACTORY,	/*恢复出厂设置*/

	SYS_OPS_MAX,
};

struct HwInfoBlock{
    unsigned int 	mask1;
    unsigned int 	lens;
    char			content[HWINFO_CONTENT_LENS];
    unsigned int 	mask2;
};

struct HwInfoLargeBlock{
    unsigned int 	mask1;
    unsigned int 	lens;
    char			*content;
    unsigned int 	mask2;
	unsigned int	total_size;
};

/*return: success: 0, error: 错误号, 请确保content数组长度大于100byte
 *type: 要读取的块(sn/pn)
 *lens:读到的content数据长度
 *content:读取到的content数据
 */
int hwinfoBlockGet(enum HWINFO_BLOCK type, unsigned int *lens, char *content);

/*return: success: 0, error: 错误号
 *type_start: 要读取的起始块
 *tpye_end:要读取的结束块
 *content: 要读取的content数据, 请确保content数组大于等于(type_end - type_start) * 128 - 28
 *lens: 要读取的content数据长度 不能大于(type_end - type_start) * 128 - 28
 */
int hwinfoLargeBlockGet(enum HWINFO_BLOCK type_start, enum HWINFO_BLOCK type_end,
							unsigned int *lens, char *content);

/*return: success: 0, error: 错误号, 请确保content数组小于等于100byte
 *type: 要写入的块(sn/pn)
 *content: 要写入的content数据
 *lens: 要写入的content数据长度 不能大于100
 */
int hwinfoBlockSet(enum HWINFO_BLOCK type, unsigned int lens, const char *content);

/*return: success: 0, error: 错误号
 *type_start: 要写入的起始块
 *tpye_end:要写入的结束块
 *content: 要写入的content数据, 请确保content数组小于等于(type_end - type_start) * 128 - 28
 *lens: 要写入的content数据长度 不能大于(type_end - type_start) * 128 - 28
 */
int hwinfoLargeBlockSet(enum HWINFO_BLOCK type_start, enum HWINFO_BLOCK type_end,
							unsigned int lens, const char *content);

int hwinfoPemRead(char *pem_path); /*读取.pem证书到pem_path中*/
int hwinfoPemWrite(char *pem_path);/*将pem_path的.pem证书保存到hwinfo块中*/

unsigned int hwinfoCrc32Verify(unsigned char *addr);

/*
 *return: success: 0, error: < 0
 *param：查询的关键字
 *val_buf：保存读取信息内容的缓存区
 *buf_size： 缓存区大小
 * 函数描述：获取cfg分区的数据，根据关键字，获取信息
 */
int get_cfg_file_value(char *param, char *val_buf, unsigned int buf_size); //获取cfg分区中key=value

/*get the hardware config wnet model config*/
int get_wnet_cfg(void);

/*get boot kernel/rootfs partition*/
bool boot_select_get(void);

/*get the hardware config file to the global variate*/
void hwinfo_init(void);

#endif /*__BC3_HWINFO__*/

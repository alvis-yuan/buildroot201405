#include <common.h>
#include <linux/mtd/mtd.h>
#include <nand.h>
#include "hwinfo.h"

#define MTD_IS_ECC_ERROR	2
#define MTD_IS_BAD_BLOCK	1
#define MTD_NOT_BAD_BLOCK	0

static volatile int hwinfo_is_inited = 0;

static unsigned int hwinfo1_seek = HWINFO_FLASH_ADDR + HWINFO1_SEEK_START;
static unsigned int hwinfo2_seek = HWINFO_FLASH_ADDR + HWINFO2_SEEK_START;

extern int sfc_nand_load(unsigned int src_addr, unsigned int count, unsigned int dst_addr);
extern int sfc_nand_block_check(unsigned int src_addr, unsigned int count, unsigned int dst_addr);

static unsigned int crc32_verify(unsigned char *buffer, unsigned int size)
{
	static unsigned int crc_table[256];
	static char flag=1;
	unsigned int crc = 0xFFFFFFFF;
    unsigned int i, j, c;

	if(flag) {
		for (i = 0; i < 256; i++) {
			c = i;
			for (j = 0; j < 8; j++) {
				if (c & 1) c = 0xedb88320 ^ (c >> 1);
				else c = c >> 1;
			}
			crc_table[i] = c;
		}
		flag = 0;
	}

    for (i = 0; i < size; i++) {
        crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
    }

    return ~crc ;
}
unsigned int hwinfoCrc32Verify(unsigned char *addr)
{
	return crc32_verify(addr, HWINFO_BLOCK_LENS * HWINFO_BLOCK_CRC);
}

void hwinfoStatusSet(int val)
{
	hwinfo_is_inited = !!val;
}

int hwinfoStatusGet(void)
{
	return hwinfo_is_inited;
}

int hwinfoBlockGet(enum HWINFO_BLOCK type, unsigned int *lens, char *content)
{
	if((lens == NULL) || (content == NULL) || (type>= HWINFO_BLOCK_MAX)) {
		return -1000;
	}
	if(hwinfoStatusGet() == 0) {
		return -1001;
	}

	*lens = simple_strtol(&((char *)HWINFO_VA_MEMORY_ADDR)[type * HWINFO_BLOCK_LENS + HWINFO_SIZE_SEEK], NULL, 10);
	memcpy(content, &((char *)HWINFO_VA_MEMORY_ADDR)[type * HWINFO_BLOCK_LENS + HWINFO_CONTENT_SEEK], *lens);

	return 0;
}

int hwinfoLargeBlockGet(enum HWINFO_BLOCK type_start, enum HWINFO_BLOCK type_end,
							unsigned int *lens, char *content)
{
	if((lens == NULL) || (content == NULL) ||(type_start >= type_end) ||(type_end >= HWINFO_BLOCK_MAX)) {
		return -1000;
	}
	if(hwinfoStatusGet() == 0) {
		return -1001;
	}

	*lens = simple_strtol(&((char *)HWINFO_VA_MEMORY_ADDR)[type_start * HWINFO_BLOCK_LENS + HWINFO_SIZE_SEEK], NULL, 10);
	memcpy(content, &((char *)HWINFO_VA_MEMORY_ADDR)[type_start * HWINFO_BLOCK_LENS + HWINFO_CONTENT_SEEK], *lens);

	return 0;
}

//return: false -- Primary partition; true  -- Backup partition
bool boot_select_get(void)
{
	unsigned int lens = 0;
	int boot_select = 0;

	hwinfoBlockGet(HWINFO_BLOCK_BOOT_SELECT, &lens, &boot_select);
	printf("%s: select lens:%d, boot_select: %d\n", __func__,lens, boot_select);
	
    return (boot_select != 0 ? true : false);
}

void hwinfo_init(void)
{

    unsigned int hwinfo1_mem_addr = CONFIG_SYS_TEXT_BASE;
    unsigned int hwinfo2_mem_addr = CONFIG_SYS_TEXT_BASE + MIN(HWINFO_FLASH_DATA_SIZE, HWINFO_VA_MEMORY_SIZE);
	unsigned int hwinfo1_up_count = 0;
	unsigned int hwinfo2_up_count = 0;
	bool hwinfo1_right = false;
	bool hwinfo2_right = false;
	unsigned int data_crc1 = 0;
	unsigned int data_crc2 = 0;
	unsigned int u_tmp1;
	unsigned int u_tmp2;
	unsigned int ll_tmp;
	int ret;

	for(ll_tmp = hwinfo1_seek; ll_tmp < (HWINFO_FLASH_ADDR + HWINFO1_SEEK_MAX); ll_tmp += HWINFO1_SEEK_STEP) {
        ret = sfc_nand_block_check(ll_tmp, HWINFO1_SEEK_STEP, hwinfo1_mem_addr);
		if(ret == MTD_IS_BAD_BLOCK || ret == MTD_IS_ECC_ERROR) {
			continue;
		} else if(ret == MTD_NOT_BAD_BLOCK) {   //当前返回值只有0、1、2
			hwinfo1_seek = ll_tmp;
			break;
		}else if(ret < 0) {
			printf("hwinfo1 get_bad_block ret error,ret=%d\n", ret);
			hwinfo1_seek = HWINFO_FLASH_ADDR + HWINFO1_SEEK_MAX; /*error addr*/
		} else {
			printf("hwinfo1 get_bad_block unknow ret=%d\n", ret);
		}
	}
    
	for(ll_tmp = hwinfo2_seek; ll_tmp < (HWINFO_FLASH_ADDR + HWINFO2_SEEK_MAX); ll_tmp += HWINFO2_SEEK_STEP) {
        ret = sfc_nand_block_check(ll_tmp, HWINFO2_SEEK_STEP, hwinfo2_mem_addr);
		if(ret == MTD_IS_BAD_BLOCK || ret == MTD_IS_ECC_ERROR) {
			continue;
		} else if(ret == MTD_NOT_BAD_BLOCK) {   //当前返回值只有0、1、2
			hwinfo2_seek = ll_tmp;
			break;
		}else if(ret < 0) {
			printf("hwinfo2 get_bad_block ret error,ret=%d\n", ret);
			hwinfo2_seek = HWINFO_FLASH_ADDR + HWINFO2_SEEK_MAX; /*error addr*/
		} else {
			printf("hwinfo2 get_bad_block unknow ret=%d\n", ret);
		}
	}

	if((hwinfo1_seek >= (HWINFO_FLASH_ADDR + HWINFO1_SEEK_MAX))
		|| (hwinfo2_seek >= (HWINFO_FLASH_ADDR + HWINFO2_SEEK_MAX))) {
		printf("hwinfo block error: 1seek: %lld, 2seek:%lld==\n", hwinfo1_seek, hwinfo2_seek);
		memset((char *)HWINFO_VA_MEMORY_ADDR, 0, HWINFO_VA_MEMORY_SIZE);
		return;
	} else {
		printf("hwinfo block ok: 1seek:0x%x, 2seek:0x%x\n", (unsigned int)hwinfo1_seek, (unsigned int)hwinfo2_seek);
	}

    sfc_nand_load(hwinfo1_seek, MIN(HWINFO_FLASH_DATA_SIZE, HWINFO_VA_MEMORY_SIZE), hwinfo1_mem_addr);
	sfc_nand_load(hwinfo2_seek, MIN(HWINFO_FLASH_DATA_SIZE, HWINFO_VA_MEMORY_SIZE), hwinfo2_mem_addr);
    
	u_tmp1 = hwinfoCrc32Verify((unsigned char *)hwinfo1_mem_addr);
	u_tmp2 = simple_strtoul(&((unsigned char *)hwinfo1_mem_addr)[HWINFO_BLOCK_LENS * HWINFO_BLOCK_CRC + HWINFO_CONTENT_SEEK], NULL, 16);
	if(u_tmp1 != u_tmp2) {  /*crc32 failed*/
		hwinfo1_right = false;
		printf("hwinfo1 crc32 failed: data crc:0x%x, store crc:0x%x\n", u_tmp1, u_tmp2);
	} else {
		hwinfo1_right = true;
		data_crc1 = u_tmp1;
		hwinfo1_up_count = simple_strtoul(&((unsigned char *)hwinfo1_mem_addr)[HWINFO_BLOCK_LENS * HWINFO_BLOCK_UPDATE_COUNT + HWINFO_CONTENT_SEEK], NULL, 16);
	}

	u_tmp1 = hwinfoCrc32Verify((unsigned char *)hwinfo2_mem_addr);
	u_tmp2 = simple_strtoul(&((unsigned char *)hwinfo2_mem_addr)[HWINFO_BLOCK_LENS * HWINFO_BLOCK_CRC + HWINFO_CONTENT_SEEK], NULL, 16);
	if(u_tmp1 != u_tmp2) {  /*crc32 failed*/
		printf("hwinfo2 crc32 failed: data crc:0x%x, store crc:0x%x\n", u_tmp1, u_tmp2);
		hwinfo2_right = false;
	} else {
		hwinfo2_right = true;
		data_crc2 = u_tmp1;
		hwinfo2_up_count = simple_strtoul(&((unsigned char *)hwinfo2_mem_addr)[HWINFO_BLOCK_LENS * HWINFO_BLOCK_UPDATE_COUNT + HWINFO_CONTENT_SEEK], NULL, 16);
	}
	if((hwinfo1_right == false) && (false == hwinfo2_right)) {
		memset((unsigned char *)HWINFO_VA_MEMORY_ADDR, 0, HWINFO_VA_MEMORY_SIZE);
		printf("hwinfo crc32 verify failed, no right data found\n");
	} else if(((hwinfo1_right == false) && (true == hwinfo2_right))
		||((hwinfo1_right == true) && (true == hwinfo2_right) && (hwinfo1_up_count < hwinfo2_up_count))) {
		memcpy((unsigned char *)HWINFO_VA_MEMORY_ADDR, (unsigned char *)hwinfo2_mem_addr, HWINFO_VA_MEMORY_SIZE);
		printf("hwinfo2 crc32 verify success, data crc:0x%x\n", data_crc2);
		hwinfoStatusSet(1);
	} else {
		memcpy((unsigned char *)HWINFO_VA_MEMORY_ADDR, (unsigned char *)hwinfo1_mem_addr, HWINFO_VA_MEMORY_SIZE);
		printf("hwinfo1 crc32 verify success, data crc:0x%x\n", data_crc1);
		hwinfoStatusSet(1);
	}
}

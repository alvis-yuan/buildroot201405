#ifndef __NAND_COMMON_H
#define __NAND_COMMON_H
#include <linux/types.h>
#include <asm/arch/sfc.h>
#include <asm/arch/spinand.h>
#include <ubi_uboot.h>

#define GET_WRITE_STATUS	1
#define GET_ECC_STATUS		2
#define GET_ERASE_STATUS	3

/* page number of convert plane */
#define NAND_PLANE_PAGES	64

/* select plane to convert columnaddr */
#define CONVERT_COL_ADDR(pageaddr, columnaddr) ({					\
	uint32_t plane_flag = (pageaddr >> (ffs(NAND_PLANE_PAGES) - 1)) & 0x1;		\
	if (plane_flag)	{								\
		columnaddr = columnaddr | ( plane_flag << 12);				\
	}										\
	columnaddr;									\
})

#define DEVICE_ID_STRUCT(id, name_string, parameter) {  \
                .id_device = id,			\
                .name = name_string,                    \
		.param = parameter,			\
}

#define CMD_INFO(_CMD, COMMAND, DUMMY_BIT, ADDR_LEN, TRANSFER_MODE) {	\
	_CMD.cmd = COMMAND;			\
	_CMD.dummy_byte = DUMMY_BIT;		\
	_CMD.addr_nbyte = ADDR_LEN;		\
	_CMD.transfer_mode = TRANSFER_MODE;	\
}

#define ST_INFO(_ST, COMMAND, BIT_SHIFT, MASK, VAL, LEN, DUMMY_BIT) {	\
	_ST.cmd = COMMAND;		\
	_ST.bit_shift = BIT_SHIFT;	\
	_ST.mask = MASK;		\
	_ST.val = VAL;			\
	_ST.len = LEN;			\
	_ST.dummy = DUMMY_BIT;		\
}

/*
 * cdt params
 */
#define CDT_PARAMS_INIT(cdt_params) {	\
	/* read to cache */							\
	CMD_INFO(cdt_params.r_to_cache, SPINAND_CMD_PARD, 0, 3, TM_STD_SPI);	\
	/* standard read from cache */						\
	CMD_INFO(cdt_params.standard_r, SPINAND_CMD_FRCH, 8, 2, TM_STD_SPI);	\
	/* quad read from cache*/						\
	CMD_INFO(cdt_params.quad_r, SPINAND_CMD_RDCH_X4, 8, 2, TM_QI_QO_SPI);	\
	/* standard write to cache*/						\
	CMD_INFO(cdt_params.standard_w_cache, SPINAND_CMD_PRO_LOAD, 0, 2, TM_STD_SPI);	\
	/* quad write to cache*/						\
	CMD_INFO(cdt_params.quad_w_cache, SPINAND_CMD_PRO_LOAD_X4, 0, 2, TM_QI_QO_SPI);	\
	/* write exec */							\
	CMD_INFO(cdt_params.w_exec, SPINAND_CMD_PRO_EN, 0, 3, TM_STD_SPI);	\
	/* block erase */							\
	CMD_INFO(cdt_params.b_erase, SPINAND_CMD_ERASE_128K, 0, 3, TM_STD_SPI);	\
	/* write enable */							\
	CMD_INFO(cdt_params.w_en, SPINAND_CMD_WREN, 0, 0, TM_STD_SPI);		\
	\
	/* get frature wait oip not busy */					\
	ST_INFO(cdt_params.oip, SPINAND_CMD_GET_FEATURE, 0, 0x1, 0x0, 1, 0);	\
}


int32_t nand_common_get_feature(struct sfc_flash *flash, uint8_t flag);
int32_t nand_get_ecc_conf(struct sfc_flash *flash, uint8_t addr);

#endif

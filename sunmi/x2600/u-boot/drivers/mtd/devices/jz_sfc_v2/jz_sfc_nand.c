/*
 * jz mtd spi nand driver probe interface
 *
 *  Copyright (C) 2013 Ingenic Semiconductor Co., LTD.
 *  Author: cli <chen.li@ingenic.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later versio.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <config.h>
#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <nand.h>
#include <linux/list.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <asm/io.h>
#include <asm/arch/spinand.h>
#include "jz_sfc_common.h"
#include "./nand_device/nand_common.h"

#ifdef CONFIG_BURNER
#include <cloner/cloner.h>
static unsigned char *readback_buf = NULL;
#endif

#ifdef MTDIDS_DEFAULT
static const char *const mtdids_default = MTDIDS_DEFAULT;
#else
static const char *const mtdids_default = "nand0:nand";
#endif

static LIST_HEAD(nand_list);
static struct sfc_flash *flash;

/*struct nand_param_from_burner nand_param_from_burner;*/
struct jz_sfcnand_burner_param jz_sfc_nand_burner_param;

static int sfcnand_block_checkbad(struct mtd_info *mtd, loff_t ofs,int getchip,int allowbbt);
static int jz_sfcnand_block_markbad(struct mtd_info *mtd, loff_t ofs);

static struct nand_ecclayout gd5f_ecc_layout_128 = {
	.oobavail = 0,
};

static int32_t jz_sfc_nand_erase_blk(struct sfc_flash *flash, uint32_t pageaddr)
{
	struct jz_sfcnand_flashinfo *nand_info = flash->flash_info;
	struct jz_sfcnand_ops *ops = nand_info->ops;
	struct sfc_cdt_xfer xfer;
	int32_t ret = 0;

	memset(&xfer, 0, sizeof(xfer));

	/* set index */
	xfer.cmd_index = NAND_ERASE_WRITE_ENABLE;

	/* set addr */
	xfer.rowaddr = pageaddr;
	xfer.staaddr0 = SPINAND_ADDR_STATUS;

	/* set transfer config */
	xfer.dataen = DISABLE;

	if(sfc_sync_cdt(flash->sfc, &xfer)) {
		printf( "sfc_sync_cdt error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}

	/* get status to be sure nand write completed */
	ret = ops->get_feature(flash, GET_ERASE_STATUS);
	if(ret){
		printf("Erase error,get state error ! %s %s %d \n",__FILE__,__func__,__LINE__);
	}

	return ret;
}

static int jz_sfc_nand_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	//struct sfc_flash *flash = TO_SFC_FLASH(mtd);
	uint32_t addr = (uint32_t)instr->addr;
	uint32_t end;
	int32_t ret;

	if(addr % mtd->erasesize) {
		printf("ERROR:%s line %d eraseaddr no align\n", __func__,__LINE__);
		return -EINVAL;
	}
	end = addr + instr->len;
	instr->state = MTD_ERASING;
	while (addr < end) {
		if((ret = jz_sfc_nand_erase_blk(flash, addr / mtd->writesize))) {
			printf("spi nand erase error blk id  %d !\n",addr / mtd->erasesize);
			instr->state = MTD_ERASE_FAILED;
			goto erase_exit;
		}
		addr += mtd->erasesize;
	}


	instr->state = MTD_ERASE_DONE;
erase_exit:
	//mtd_erase_callback(instr);
	return ret;
}

static int32_t jz_sfc_nand_write(struct sfc_flash *flash, const u_char *buffer, uint32_t pageaddr, uint32_t columnaddr, size_t len)
{
	struct jz_sfcnand_flashinfo *nand_info = flash->flash_info;
	struct jz_sfcnand_ops *ops = nand_info->ops;
	struct sfc_cdt_xfer xfer;
	int32_t ret = 0;

	memset(&xfer, 0, sizeof(xfer));

	/* set Index */
	if(nand_info->param.need_quad){
		xfer.cmd_index = NAND_QUAD_WRITE_ENABLE;
	}else{
		xfer.cmd_index = NAND_STANDARD_WRITE_ENABLE;
	}

	/* set addr */
	xfer.rowaddr = pageaddr;

	if(nand_info->param.plane_select){
		xfer.columnaddr = CONVERT_COL_ADDR(pageaddr, columnaddr);
	}else{
		xfer.columnaddr = columnaddr;
	}

	xfer.staaddr0 = SPINAND_ADDR_STATUS;

	/* set transfer config */
	xfer.dataen = ENABLE;
	xfer.config.datalen = len;
	xfer.config.data_dir = GLB_TRAN_DIR_WRITE;
	xfer.config.ops_mode = CPU_OPS;
	xfer.config.buf = (u_char *)buffer;

	if(sfc_sync_cdt(flash->sfc, &xfer)) {
		printf("sfc_sync_cdt error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}

	/* get status to be sure nand write completed */
	ret = ops->get_feature(flash, GET_WRITE_STATUS);

	return  ret;
}

static int32_t jz_sfc_nand_read(struct sfc_flash *flash, int32_t pageaddr, int32_t columnaddr, uint8_t *buffer, size_t len)
{
	struct jz_sfcnand_flashinfo *nand_info = flash->flash_info;
	struct jz_sfcnand_ops *ops = nand_info->ops;
	struct sfc_cdt_xfer xfer;

	memset(&xfer, 0, sizeof(xfer));

	/* set Index */
	if(nand_info->param.need_quad){
		xfer.cmd_index = NAND_QUAD_READ_TO_CACHE;
	}else{
		xfer.cmd_index = NAND_STANDARD_READ_TO_CACHE;
	}

	/* set addr */
	xfer.rowaddr = pageaddr;

	if(nand_info->param.plane_select){
		xfer.columnaddr = CONVERT_COL_ADDR(pageaddr, columnaddr);
	}else{
		xfer.columnaddr = columnaddr;
	}

	xfer.staaddr0 = SPINAND_ADDR_STATUS;

	/* set transfer config */
	xfer.dataen = ENABLE;
	xfer.config.datalen = len;
	xfer.config.data_dir = GLB_TRAN_DIR_READ;
	xfer.config.ops_mode = CPU_OPS;
	xfer.config.buf = buffer;

	if(sfc_sync_cdt(flash->sfc, &xfer)) {
		printf("sfc_sync_cdt error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}

	/* get status to check nand ecc status */
	return ops->get_feature(flash, GET_ECC_STATUS);
}

static int jz_sfcnand_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	uint32_t pagesize = mtd->writesize;
	uint32_t pageaddr;
	uint32_t columnaddr;
	uint32_t rlen;
	int32_t ret = 0, reterr = 0, ret_eccvalue = 0;

	*retlen = 0;
	while((int)len > 0) {
		pageaddr = (uint32_t)from / pagesize;
		columnaddr = (uint32_t)from % pagesize;
		rlen = min_t(uint32_t, len, pagesize - columnaddr);
		ret = jz_sfc_nand_read(flash, pageaddr, columnaddr, buf, rlen);
		if(ret < 0) {
			printf("%s %s %d: jz_sfc_nand_read error, ret = %d, \
				pageaddr = %u, columnaddr = %u, rlen = %u\n",
				__FILE__, __func__, __LINE__,
				ret, pageaddr, columnaddr, rlen);
			reterr = ret;
			if(ret == -EIO)
				break;
		} else if (ret > 0) {
			printf("%s %s %d: jz_sfc_nand_read, ecc value = %d\n",
				__FILE__, __func__, __LINE__, ret);
			ret_eccvalue = ret;
		}

		len -= rlen;
		from += rlen;
		buf += rlen;
		*retlen += rlen;
	}

	return reterr ? reterr : (ret_eccvalue ? ret_eccvalue : ret);
}

static int jz_sfcnand_write_oob(struct mtd_info *mtd, loff_t addr, struct mtd_oob_ops *ops)
{
	uint32_t oob_addr = (uint32_t)addr;
	int32_t ret;

#ifdef CONFIG_FLASH_RESERVED_PART
	if(check_partition_is_writable(flash, oob_addr, (uint32_t)ops->len))
		return -EROFS;
#endif

	debug("write oob_addr %x, datalen %d ooboff %d, ooblen %d\n", oob_addr, ops->len, ops->ooboffs, ops->ooblen);

	if(ops->datbuf && ops->len) {
		if((ret = jz_sfc_nand_write(flash, ops->datbuf, oob_addr / mtd->writesize, 0, ops->len))) {
			printf( "spi nand write oob data area error %s %s %d \n",__FILE__,__func__,__LINE__);
			goto write_oob_exit;
		}
	}
	if(ops->oobbuf && ops->ooblen) {
		if((ret = jz_sfc_nand_write(flash, ops->oobbuf, oob_addr / mtd->writesize, mtd->writesize + ops->ooboffs, ops->ooblen))) {
			printf( "spi nand write oob oob area error %s %s %d \n",__FILE__,__func__,__LINE__);
			goto write_oob_exit;
		}
	}
	ops->retlen = ops->len;
	ops->oobretlen = ops->ooblen;

write_oob_exit:
	return ret;
}

static int sfcnand_block_isbad(struct mtd_info *mtd,loff_t ofs)
{
	int ret;
	ret = sfcnand_block_checkbad(mtd, ofs,1, 0);
	return ret;
}

static int sfcnand_block_markbad(struct mtd_info *mtd,loff_t ofs)
{
	struct nand_chip *chip = mtd->priv;
	int ret;

	ret = sfcnand_block_isbad(mtd, ofs);
	if (ret) {
		/* If it was bad already, return success and do nothing */
		if (ret > 0)
			return 0;
		return ret;
	}

	return chip->block_markbad(mtd, ofs);
}

static int jz_sfcnand_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	int ret;
#ifdef CONFIG_FLASH_RESERVED_PART
	if(check_partition_is_writable(flash, (uint32_t)instr->addr, (uint32_t)instr->len))
		return -EROFS;
#endif

	if((ret = jz_sfc_nand_erase(mtd, instr))) {
		printf("WARNING: block %d erase fail !\n",(uint32_t)instr->addr / mtd->erasesize);
		if((ret = jz_sfcnand_block_markbad(mtd, instr->addr))) {
			printf("mark bad block error, there will occur error,so exit !\n");
			return -1;
		}
	}
	instr->state = MTD_ERASE_DONE;
	return 0;
}

static int32_t jz_sfcnand_read_oob(struct mtd_info *mtd, loff_t from, struct mtd_oob_ops *ops)
{
	uint32_t addr = (uint32_t)from;
	uint32_t pageaddr = addr / mtd->writesize;
	int32_t ret = 0, ret_eccvalue = 0;

	if(ops->datbuf && ops->len) {
		ret = jz_sfc_nand_read(flash, pageaddr, 0, ops->datbuf, ops->len);
		if(ret < 0) {
			printf("spi nand read error %s %s %d ,ret = %d\n", __FILE__, __func__,  __LINE__, ret);
			if(ret == -EIO) {
				return ret;
			} else {
				ret_eccvalue = ret;
			}
		}

		ops->retlen = ops->len;
	}
	if(ops->oobbuf && ops->ooblen){
		ret = jz_sfc_nand_read(flash, pageaddr, mtd->writesize + ops->ooboffs, ops->oobbuf, ops->ooblen);
		if(ret < 0)
		            printf("%s %s %d : spi nand read oob error ,ret= %d , oob addr %x, ooboffs %d, ooblen %d\n",
					__FILE__, __func__, __LINE__, ret, addr, ops->ooboffs, ops->ooblen);

		if(ret != -EIO)
		            ops->oobretlen = ops->ooblen;
	}
	return ret ? ret : ret_eccvalue;
}

static int badblk_check(int len,unsigned char *buf)
{
	int j = 0;
	unsigned char *check_buf = buf;

	for(j = 0; j < len; j++){
	    if(check_buf[j] != 0xff){
		return 1;
	    }
	}

	return 0;
}

static int sfcnand_block_checkbad(struct mtd_info *mtd, loff_t ofs,int getchip,int allowbbt)
{
	struct nand_chip *chip = mtd->priv;
	int ret = 0;

	if (chip->bbt && (chip->options & NAND_BBT_SCANNED))
		ret = nand_isbad_bbt(mtd, ofs, allowbbt);
	else
		ret = chip->block_bad(mtd, ofs,getchip);

	return ret;
}


static int jz_sfcnand_block_bad_check(struct mtd_info *mtd, loff_t ofs,int getchip)
{
	int check_len = 1;
	unsigned char check_buf[] = {0xff, 0xff};
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct mtd_oob_ops ops;

	memset(&ops, 0, sizeof(ops));
	if (chip->options & NAND_BUSWIDTH_16)
		check_len = 2;

	ops.oobbuf = check_buf;
	ops.ooblen = check_len;
	jz_sfcnand_read_oob(mtd, ofs, &ops);

	if(badblk_check(check_len, check_buf))
		return 1;
	return 0;
}

static int jz_sfcnand_write(struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
	uint32_t pagesize = mtd->writesize;
	uint32_t pageaddr;
	uint32_t columnaddr;
	uint32_t wlen;
	int32_t ret;

#ifdef CONFIG_FLASH_RESERVED_PART
	if(check_partition_is_writable(flash, (uint32_t)to, (uint32_t)len))
		return -EROFS;
#endif

	while(len) {
		pageaddr = (uint32_t)to / pagesize;
		columnaddr = (uint32_t)to % pagesize;
		wlen = min_t(uint32_t, pagesize - columnaddr, len);

		if((ret = jz_sfc_nand_write(flash, buf, pageaddr, columnaddr, wlen))) {
			printf("%s %s %d : spi nand write fail, ret = %d, \
				pageaddr = %u, columnaddr = %u, wlen = %u\n",
				__FILE__, __func__, __LINE__, ret,
				pageaddr, columnaddr, wlen);
			break;
		}

#ifdef CONFIG_BURNER
		if(debug_args->write_back_chk) {
			if(!readback_buf) {
				readback_buf = (unsigned char *)malloc(wlen);
				if(!readback_buf) {
					printf("burn read back buffer malloc failed!\n");
					ret = -ENOMEM;
					break;
				}
			}
			memset(readback_buf, 0, wlen);
			ret = jz_sfc_nand_read(flash, pageaddr, columnaddr, readback_buf, wlen);
			if(ret < 0) {
				printf("%s %s %d: jz_sfc_nand_read error, ret = %d, \
						pageaddr = %u, columnaddr = %u, rlen = %u\n",
						__FILE__, __func__, __LINE__,
						ret, pageaddr, columnaddr, wlen);
				if(ret == -EIO)
					break;
			} else if (ret > 0) {
				printf("%s %s %d: jz_sfc_nand_read, ecc value = %d\n",
						__FILE__, __func__, __LINE__, ret);
			}

			ret = buf_compare(buf, readback_buf, wlen, (uint32_t)to);
			if(ret != 0) {
				printf("%s %s %d: burn read back compare error!\n",
						__FILE__, __func__, __LINE__);
				break;
			}
		}
#endif

		*retlen += wlen;
		len -= wlen;
		to += wlen;
		buf += wlen;
	}

	return ret;
}

static void mtd_sfcnand_init(struct mtd_info *mtd)
{
	mtd->_erase = jz_sfcnand_erase;
	mtd->_point = NULL;
	mtd->_unpoint = NULL;
	mtd->_read = jz_sfcnand_read;
	mtd->_write = jz_sfcnand_write;
	mtd->_read_oob = jz_sfcnand_read_oob;
	mtd->_write_oob = jz_sfcnand_write_oob;
	mtd->_lock = NULL;
	mtd->_unlock = NULL;
	mtd->_block_isbad = sfcnand_block_isbad;
	mtd->_block_markbad = sfcnand_block_markbad;
}

static int jz_sfcnand_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	struct nand_chip *chip = mtd->priv;
	uint8_t buf[2] = { 0, 0 };
	int res, ret = 0, i = 0;
	int write_oob = !(chip->bbt_options & NAND_BBT_NO_OOB_BBM);

	/* Write bad block marker to OOB */
	if (write_oob) {
		struct mtd_oob_ops ops;
		loff_t wr_ofs = ofs;

		ops.datbuf = NULL;
		ops.oobbuf = buf;
		ops.ooboffs = chip->badblockpos;
		if (chip->options & NAND_BUSWIDTH_16) {
			ops.ooboffs &= ~0x01;
			ops.len = ops.ooblen = 2;
		} else {
			ops.len = ops.ooblen = 1;
		}
		ops.mode = MTD_OPS_PLACE_OOB;

		/* Write to first/last page(s) if necessary */
		if (chip->bbt_options & NAND_BBT_SCANLASTPAGE)
			wr_ofs += mtd->erasesize - mtd->writesize;
		do {
			res = jz_sfcnand_write_oob(mtd, wr_ofs, &ops);
			if (!ret)
				ret = res;

			i++;
			wr_ofs += mtd->writesize;
		} while ((chip->bbt_options & NAND_BBT_SCAN2NDPAGE) && i < 2);
	}

	/* Update flash-based bad block table */
	if (chip->bbt_options & NAND_BBT_USE_FLASH) {
		res = nand_update_bbt(mtd, ofs);
	}

	return ret;

}

static void get_partition_from_spinand(struct sfc_flash *flash)
{
	size_t retlen;
	jz_sfcnand_read(flash->mtd, CONFIG_SPIFLASH_PART_OFFSET, sizeof(struct jz_sfcnand_burner_param) - 4, &retlen, (u_char *)&jz_sfc_nand_burner_param);
	jz_sfc_nand_burner_param.partition = malloc(sizeof(struct jz_sfcnand_partition) * jz_sfc_nand_burner_param.partition_num);
	jz_sfcnand_read(flash->mtd, CONFIG_SPIFLASH_PART_OFFSET + sizeof(struct jz_sfcnand_burner_param) - 4,
			sizeof(struct jz_sfcnand_partition) * jz_sfc_nand_burner_param.partition_num, &retlen,
			(u_char *)jz_sfc_nand_burner_param.partition);
#ifdef DEBUG
	{
		int32_t i;
	for(i = 0; i < jz_sfc_nand_burner_param.partition_num; i++) {
		printf("name = %s\n", jz_sfc_nand_burner_param.partition[i].name);
		printf("size = %x\n", jz_sfc_nand_burner_param.partition[i].size);
		printf("offset = %x\n", jz_sfc_nand_burner_param.partition[i].offset);
		printf("mask_flags = %x\n", jz_sfc_nand_burner_param.partition[i].mask_flags);
	}
	}
#endif
}

static int32_t sfc_nand_reset(void)
{
	struct sfc_cdt_xfer xfer;

	memset(&xfer, 0, sizeof(xfer));

	/* set Index */
	xfer.cmd_index = NAND_RESET;

	/* set addr */

	/* set transfer config */
	xfer.dataen = DISABLE;

	if(sfc_sync_cdt(flash->sfc, &xfer)) {
		printf("%s %s %d: sfc sync cdt failed!\n", __FILE__, __func__, __LINE__);
		return -EIO;
	}

	return 0;
}

static int32_t jz_sfc_nand_try_id(struct sfc_flash *flash, struct jz_sfcnand_flashinfo *nand_info)
{
	struct jz_sfcnand_device *nand_device;
	struct sfc_cdt_xfer xfer;
	uint8_t id_buf[3] = {0};
	unsigned short index[2] = {NAND_TRY_ID, NAND_TRY_ID_DMY};
	uint8_t i = 0;
	struct device_id_struct *device_id = NULL;
	int32_t id_count = 0;


	for(i = 0; i < 2; i++) {
		memset(id_buf,0,sizeof(id_buf));
		memset(&xfer, 0, sizeof(xfer));

		/* set index */
		xfer.cmd_index = index[i];

		/* set addr */
		xfer.rowaddr = 0;

		/* set transfer config */
		xfer.dataen = ENABLE;
		xfer.config.datalen = sizeof(id_buf);
		xfer.config.data_dir = GLB_TRAN_DIR_READ;
		xfer.config.ops_mode = CPU_OPS;
		xfer.config.buf = id_buf;

		if(sfc_sync_cdt(flash->sfc, &xfer)) {
			printf("sfc_sync_cdt error ! %s %s %d\n",__FILE__,__func__,__LINE__);
			return -EIO;
		}

		printf("id_manufactory = %x, id_device1 %x id_device2 %x\n", id_buf[0], id_buf[1],id_buf[2]);
		list_for_each_entry(nand_device, &nand_list, list) {
			if(nand_device->id_manufactory == id_buf[0]) {
				device_id = nand_device->id_device_list;
				id_count = nand_device->id_device_count;
				while(id_count--) {
					if (device_id->id_device >0x0 && device_id->id_device <= 0xff &&
									(device_id->id_device == id_buf[1])) {
						nand_info->id_manufactory = id_buf[0];
						nand_info->id_device = id_buf[1];
						nand_info->param = *device_id->param;
			         		goto found_param;
					}

					else if (device_id->id_device >0xff && device_id->id_device <= 0xffff &&
									device_id->id_device == (id_buf[2] | (id_buf[1]<<8))) {
						nand_info->id_manufactory = id_buf[0];
						nand_info->id_device = id_buf[1]<<8;
						nand_info->id_device  |= id_buf[2];
						nand_info->param = *device_id->param;
						goto found_param;
					}
					device_id++;
				}

			}
		}
		udelay(500);
	}

	if(!nand_info->id_manufactory && !nand_info->id_device) {
		printf("ERROR!: don`t support this nand manufactory, please add nand driver, id_manufactory 0x%x id_device 0x%x\n", id_buf[0], id_buf[1]);
		return -ENODEV;
	}

found_param:
	printf("Found nand: id_manufactory: 0x%02x id_device: 0x%04x\n", nand_info->id_manufactory, nand_info->id_device);

	/* fill manufactory special operation and cdt params */
	nand_info->ops = &nand_device->ops;
	nand_info->cdt_params = nand_info->ops->get_cdt_params(flash, nand_info->id_device);

	if (!nand_info->ops->get_feature) {
		if (!nand_info->ops->deal_ecc_status) {
			printf("ERROR:xxx_nand.c \"get_feature()\" and \"deal_ecc_status()\" not define.\n");
			return -ENODEV;
		} else {
			nand_info->ops->get_feature = nand_common_get_feature;
			printf("use nand common get feature interface!\n");
		}
	} else {
		printf("use nand private get feature interface!\n");
	}

	return 0;
}

int jz_sfcnand_register(struct jz_sfcnand_device *flash) {

	if (!flash)
		return -EINVAL;
	list_add_tail(&flash->list, &nand_list);
	return 0;
}

static int32_t sfc_nand_get_feature(struct sfc_flash *flash, uint8_t addr, uint8_t *val)
{
	struct sfc_cdt_xfer xfer;
	memset(&xfer, 0, sizeof(xfer));

	/* set index */
	xfer.cmd_index = NAND_GET_FEATURE;

	/* set addr */
	xfer.staaddr0 = addr;

	/* set transfer config */
	xfer.dataen = ENABLE;
	xfer.config.datalen = 1;
	xfer.config.data_dir = GLB_TRAN_DIR_READ;
	xfer.config.ops_mode = CPU_OPS;
	xfer.config.buf = val;

	if(sfc_sync_cdt(flash->sfc, &xfer)) {
		printf("sfc_sync_cdt error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}

	return 0;
}

static int32_t sfc_nand_set_feature(struct sfc_flash *flash, uint8_t addr, uint8_t val)
{
	struct sfc_cdt_xfer xfer;
	memset(&xfer, 0, sizeof(xfer));

	/* set index */
	xfer.cmd_index = NAND_SET_FEATURE;

	/* set addr */
	xfer.staaddr0 = addr;

	/* set transfer config */
	xfer.dataen = ENABLE;
	xfer.config.datalen = 1;
	xfer.config.data_dir = GLB_TRAN_DIR_WRITE;
	xfer.config.ops_mode = CPU_OPS;
	xfer.config.buf = (uint8_t *)&val;

	if(sfc_sync_cdt(flash->sfc, &xfer)) {
		printf("sfc_sync_cdt error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}

	return 0;
}

static int32_t sfc_nand_clear_write_protect(struct sfc_flash *flash)
{
	return sfc_nand_set_feature(flash, 0xa0, 0);
}

static int32_t sfc_nand_enable_ecc(struct sfc_flash *flash)
{
	int32_t ret = 0;
	uint8_t val = 0;
	if((ret = sfc_nand_get_feature(flash, 0xb0, &val))) {
		printf(" %s %s %d: sfc_nand_get_feature failed, ret = %d\n",
			__FILE__, __func__, __LINE__, ret);
		return ret;
	}

	val |= (1 << 4) | (1 << 3) | 0x1;

	if((ret = sfc_nand_set_feature(flash, 0xb0, val))) {
		printf(" %s %s %d: sfc_nand_set_feature failed, ret = %d\n",
			__FILE__, __func__, __LINE__, ret);
	}
	return ret;
}

static int32_t sfc_nand_dev_init(struct sfc_flash *flash)
{
	int32_t ret = 0;
	ret = sfc_nand_clear_write_protect(flash);
	if(ret) {
		printf("sfc_nand_clear_write_protect failed!, ret= %d\n",ret);
		return ret;
	}
	ret = sfc_nand_enable_ecc(flash);
	if(ret) {
		printf("sfc_nand_enable_ecc failed!, ret= %d\n", ret);
	}
	return ret;
}

static inline int32_t spinand_moudle_init(void)
{
	spinand_regcall_t *call = ll_entry_start(spinand_regcall_t, flash);
	int ret = 0 , i, count;

	for (i = 0, count = ll_entry_count(spinand_regcall_t, flash);
		i < count;
		call++, i++) {
	    ret = (*call)();
	    if (ret) {
		printf("jz spi nand ops init func error\n");
		break;
	    }
	}
	return ret;
}

/*
 *MK_CMD(cdt, cmd, LINK, ADDRMODE, DATA_EN)
 *MK_ST(cdt, st, LINK, ADDRMODE, ADDR_WIDTH, POLL_EN, DATA_EN, TRAN_MODE)
 */
static inline void params_to_cdt(cdt_params_t *params, struct sfc_cdt *cdt)
{
	/* 6. nand standard read */
	MK_CMD(cdt[NAND_STANDARD_READ_TO_CACHE], params->r_to_cache, 1, ROW_ADDR, DISABLE);
	MK_ST(cdt[NAND_STANDARD_READ_GET_FEATURE], params->oip, 1, STA_ADDR0, 1, ENABLE, DISABLE, TM_STD_SPI);
	MK_CMD(cdt[NAND_STANDARD_READ_FROM_CACHE], params->standard_r, 0, COL_ADDR, ENABLE);

	/* 7. nand quad read */
	MK_CMD(cdt[NAND_QUAD_READ_TO_CACHE], params->r_to_cache, 1, ROW_ADDR, DISABLE);
	MK_ST(cdt[NAND_QUAD_READ_GET_FEATURE], params->oip, 1, STA_ADDR0, 1, ENABLE, DISABLE, TM_STD_SPI);
	MK_CMD(cdt[NAND_QUAD_READ_FROM_CACHE], params->quad_r, 0, COL_ADDR, ENABLE);

	/* 8. nand standard write */
	MK_CMD(cdt[NAND_STANDARD_WRITE_ENABLE], params->w_en, 1, DEFAULT_ADDRMODE, DISABLE);
	MK_CMD(cdt[NAND_STANDARD_WRITE_TO_CACHE], params->standard_w_cache, 1, COL_ADDR, ENABLE);
	MK_CMD(cdt[NAND_STANDARD_WRITE_EXEC], params->w_exec, 1, ROW_ADDR, DISABLE);
	MK_ST(cdt[NAND_STANDARD_WRITE_GET_FEATURE], params->oip, 0, STA_ADDR0, 1, ENABLE, DISABLE, TM_STD_SPI);

	/* 9. nand quad write */
	MK_CMD(cdt[NAND_QUAD_WRITE_ENABLE], params->w_en, 1, DEFAULT_ADDRMODE, DISABLE);
	MK_CMD(cdt[NAND_QUAD_WRITE_TO_CACHE], params->quad_w_cache, 1, COL_ADDR, ENABLE);
	MK_CMD(cdt[NAND_QUAD_WRITE_EXEC], params->w_exec, 1, ROW_ADDR, DISABLE);
	MK_ST(cdt[NAND_QUAD_WRITE_GET_FEATURE], params->oip, 0, STA_ADDR0, 1, ENABLE, DISABLE, TM_STD_SPI);

	/* 10. block erase */
	MK_CMD(cdt[NAND_ERASE_WRITE_ENABLE], params->w_en, 1, DEFAULT_ADDRMODE, DISABLE);
	MK_CMD(cdt[NAND_BLOCK_ERASE], params->b_erase, 1, ROW_ADDR, DISABLE);
	MK_ST(cdt[NAND_ERASE_GET_FEATURE], params->oip, 0, STA_ADDR0, 1, ENABLE, DISABLE, TM_STD_SPI);

	/* 11. ecc status read */
	MK_CMD(cdt[NAND_ECC_STATUS_READ], params->ecc_r, 0, DEFAULT_ADDRMODE, ENABLE);

}

static inline void create_cdt_table(struct sfc_flash *flash, uint32_t flag)
{
	struct jz_sfcnand_flashinfo *nand_info = flash->flash_info;
	cdt_params_t *cdt_params;
	struct sfc_cdt sfc_cdt[INDEX_MAX_NUM];

	memset(sfc_cdt, 0, sizeof(sfc_cdt));
	if(flag == DEFAULT_CDT)
	{
		/* 1. reset */
		sfc_cdt[NAND_RESET].link = CMD_LINK(0, DEFAULT_ADDRMODE, TM_STD_SPI);
		sfc_cdt[NAND_RESET].xfer = CMD_XFER(0, DISABLE, 0, DISABLE, SPINAND_CMD_RESET);
		sfc_cdt[NAND_RESET].staExp = 0;
		sfc_cdt[NAND_RESET].staMsk = 0;

		/* 2. try id */
		sfc_cdt[NAND_TRY_ID].link = CMD_LINK(0, DEFAULT_ADDRMODE, TM_STD_SPI);
		sfc_cdt[NAND_TRY_ID].xfer = CMD_XFER(0, DISABLE, 0, ENABLE, SPINAND_CMD_RDID);
		sfc_cdt[NAND_TRY_ID].staExp = 0;
		sfc_cdt[NAND_TRY_ID].staMsk = 0;

		/* 3. try id with dummy */
		/*
		 * There are some NAND flash, try ID operation requires 8-bit dummy value to be all 0,
		 * so use 1 byte address instead of dummy here.
		 */
		sfc_cdt[NAND_TRY_ID_DMY].link = CMD_LINK(0, ROW_ADDR, TM_STD_SPI);
		sfc_cdt[NAND_TRY_ID_DMY].xfer = CMD_XFER(1, DISABLE, 0, ENABLE, SPINAND_CMD_RDID);
		sfc_cdt[NAND_TRY_ID_DMY].staExp = 0;
		sfc_cdt[NAND_TRY_ID_DMY].staMsk = 0;

		/* 4. set feature */
		sfc_cdt[NAND_SET_FEATURE].link = CMD_LINK(0, STA_ADDR0, TM_STD_SPI);
		sfc_cdt[NAND_SET_FEATURE].xfer = CMD_XFER(1, DISABLE, 0, ENABLE, SPINAND_CMD_SET_FEATURE);
		sfc_cdt[NAND_SET_FEATURE].staExp = 0;
		sfc_cdt[NAND_SET_FEATURE].staMsk = 0;

		/* 5. get feature */
		sfc_cdt[NAND_GET_FEATURE].link = CMD_LINK(0, STA_ADDR0, TM_STD_SPI);
		sfc_cdt[NAND_GET_FEATURE].xfer = CMD_XFER(1, DISABLE, 0, ENABLE, SPINAND_CMD_GET_FEATURE);
		sfc_cdt[NAND_GET_FEATURE].staExp = 0;
		sfc_cdt[NAND_GET_FEATURE].staMsk = 0;

		/* first create cdt table */
		write_cdt(flash->sfc, sfc_cdt, NAND_RESET, NAND_GET_FEATURE);
	}

	if(flag == UPDATE_CDT){
		cdt_params = nand_info->cdt_params;
		params_to_cdt(cdt_params, sfc_cdt);

		/* second create cdt table */
		write_cdt(flash->sfc, sfc_cdt, NAND_STANDARD_READ_TO_CACHE, NAND_ECC_STATUS_READ);
	}
	//dump_cdt(flash->sfc);
}

int32_t jz_sfc_nand_init()
{
	struct nand_chip *chip;
	struct mtd_info *mtd;
	struct jz_sfcnand_flashinfo *flash_info;
	int32_t ret = 0;
	uint32_t sfc_rate = 200000000;
#ifdef CONFIG_BURNER
	struct jz_sfcnand_burner_param *param = spi_args->flash_info;
#endif

	if(!flash) {
		flash = malloc(sizeof(struct sfc_flash));
		if (!flash) {
			printf("ERROR: %s %d alloc() error !\n",__func__,__LINE__);
			return -1;
		}
		memset(flash, 0, sizeof(struct sfc_flash));

#ifdef CONFIG_SFC_NAND_INIT_RATE
		sfc_rate = CONFIG_SFC_NAND_INIT_RATE;
#endif
#ifdef CONFIG_BURNER
		if(spi_args->sfc_frequency)
			sfc_rate = spi_args->sfc_frequency;
#endif
		flash->sfc = sfc_res_init(sfc_rate);

	}
	mtd = &nand_info[0];
	flash_info = calloc(sizeof(struct jz_sfcnand_flashinfo), sizeof(uint8_t));
	if(!flash_info) {
		printf("ERROR: %s %d alloc() error !\n",__func__,__LINE__);
		return -ENOMEM;
	}
	flash->flash_info = flash_info;
	flash->mtd = mtd;

	if (spinand_moudle_init()) {
		ret = -EINVAL;
		goto failed;
	}

#define THOLD   5
#define TSETUP  5
#define TSHSL_R     100
#define TSHSL_W     100

	set_flash_timing(flash->sfc, THOLD, TSETUP, TSHSL_R, TSHSL_W);

	/* Try creating default CDT table */
	create_cdt_table(flash, DEFAULT_CDT);

	if((ret = sfc_nand_reset())) {
		printf("ERR : sfc reset error!\n");
		goto failed;
	}

	if((ret = jz_sfc_nand_try_id(flash, flash_info))) {
		printf("ERR: sfc try id error!\n");
		goto failed;
	}

	/* Update to private CDT table */
	create_cdt_table(flash, UPDATE_CDT);

#ifndef CONFIG_BURNER
	/* update sfc rate */
	sfc_clk_set(flash->sfc, CONFIG_SFC_NAND_RATE);
#endif
	set_flash_timing(flash->sfc, flash_info->param.tHOLD, flash_info->param.tSETUP, flash_info->param.tSHSL_R, flash_info->param.tSHSL_W);

	if((ret = sfc_nand_dev_init(flash))) {
		printf("ERR :sfcnand device init failed!\n");
		goto failed;
	}

	mtd->size = flash_info->param.flashsize;
#ifdef CONFIG_BURNER
	if(spi_args->reserve_space)
#endif
	{
#if defined(CONFIG_JZ_SPINAND_SN)
		mtd->size -= CONFIG_SN_SIZE;
#endif
#if defined(CONFIG_JZ_SPINAND_MAC)
		mtd->size -= CONFIG_MAC_SIZE;
#endif
#if defined(CONFIG_JZ_SPINAND_LICENSE)
		mtd->size -= CONFIG_LICENSE_SIZE;
#endif
	}

#ifdef CONFIG_BURNER
	readback_buf = (unsigned char *)malloc(flash_info->param.pagesize);
	flash_info->param.need_quad = spi_args->sfc_quad_mode;

	/* for burner get pt indext */
	flash_info->partition.num_partition = param->partition_num;
	flash_info->partition.partition = &param->partition;
	{
		int part = 0;
		for (part = 0; part < flash_info->partition.num_partition; part++)
			if (flash_info->partition.partition[part].size == 0UL ||
					flash_info->partition.partition[part].size == -1UL)
				break;
		if (part != flash_info->partition.num_partition) {
			flash_info->partition.partition[part].size = mtd->size -
				flash_info->partition.partition[part].offset;
			printf("flash size 0x%x, part size 0x%x\n", flash_info->param.flashsize, flash_info->partition.partition[part].size);
		}
	}
#else
	mtd->writesize = flash_info->param.pagesize;
	get_partition_from_spinand(flash);
	flash_info->partition.num_partition =jz_sfc_nand_burner_param.partition_num;
	flash_info->partition.partition = jz_sfc_nand_burner_param.partition;
#endif

	chip = calloc(1, sizeof(struct nand_chip));
	if (!chip) {
		ret = -ENOMEM;
		goto failed;
	}
	memset(chip,0,sizeof(struct nand_chip));

	mtd->flags |= MTD_CAP_NANDFLASH;
	mtd->erasesize = flash_info->param.blocksize;
	mtd->writesize = flash_info->param.pagesize;
	mtd->oobsize = flash_info->param.oobsize;

	chip->select_chip = NULL;
	chip->badblockbits = 8;
	chip->scan_bbt = nand_default_bbt;
	chip->block_bad = jz_sfcnand_block_bad_check;
	chip->block_markbad = jz_sfcnand_block_markbad;
	chip->ecc.layout= &gd5f_ecc_layout_128; // for erase ops
	chip->bbt_erase_shift = chip->phys_erase_shift = ffs(mtd->erasesize) - 1;
	if (!(chip->options & NAND_OWN_BUFFERS))
		chip->buffers = memalign(ARCH_DMA_MINALIGN,sizeof(*chip->buffers));

	/* Set the bad block position */
	if (mtd->writesize > 512 || (chip->options & NAND_BUSWIDTH_16))
		chip->badblockpos = NAND_LARGE_BADBLOCK_POS;
	else
		chip->badblockpos = NAND_SMALL_BADBLOCK_POS;

	mtd->priv = chip;

	mtd_sfcnand_init(mtd);
	nand_register(0);

#ifdef CONFIG_FLASH_RESERVED_PART
	/*
	 * Note that this feature may conflict with flash based BBT
	 */
	ret = reserved_part_init(flash);
	if (ret) {
		printf("failed to init reserved partitions\n");
		ret = -EIO;
		goto failed;
	}
	ret = scan_reserved_part(flash, \
			flash_info->partition.partition, flash_info->partition.num_partition);
	if(ret) {
		printf("failed to scan reserved partitions\n");
		ret = -EIO;
		goto failed;
	}
#endif

	return ret;

failed:
	free(flash_info);
	free(flash);
	return ret;
}

#ifdef CONFIG_BURNER
static void mtd_sfcnand_partition_analysis(uint32_t blk_sz, uint8_t partcount, struct jz_sfcnand_partition *jz_mtd_spinand_partition)
{
	char mtdparts_env[X_ENV_LENGTH];
	char command[X_COMMAND_LENGTH];
	uint8_t part = 0;

	memset(mtdparts_env, 0, X_ENV_LENGTH);
	memset(command, 0, X_COMMAND_LENGTH);

	/*MTD part*/
	sprintf(mtdparts_env, "mtdparts=nand:");
	for (part = 0; part < partcount; part++) {
		if (jz_mtd_spinand_partition[part].size == -1UL || jz_mtd_spinand_partition[part].size == 0UL) {
			sprintf(mtdparts_env,"%s-(%s)", mtdparts_env,
					jz_mtd_spinand_partition[part].name);
			break;
		} else if (jz_mtd_spinand_partition[part].size != 0) {
			if(jz_mtd_spinand_partition[part].size % blk_sz != 0)
				printf("ERROR:the partition [%s] don't algin as block size [0x%08x] ,it will be error !\n",jz_mtd_spinand_partition[part].name, blk_sz);

			sprintf(mtdparts_env, "%s%dK@%d(%s),", mtdparts_env,
					jz_mtd_spinand_partition[part].size / 0x400,
					jz_mtd_spinand_partition[part].offset,
					jz_mtd_spinand_partition[part].name);
		} else {
			break;
		}
	}
	debug("env:mtdparts=%s\n", mtdparts_env);
	setenv("mtdids", mtdids_default);
	setenv("mtdparts", mtdparts_env);
	setenv("partition", NULL);
}


struct jz_sfcnand_partition *get_partion_index(u32 startaddr,u32 length,int *pt_index)
{
	struct jz_sfcnand_flashinfo *nand_info = flash->flash_info;
	int i;
	int ptcount = nand_info->partition.num_partition;
	struct jz_sfcnand_partition *jz_mtd_spinand_partition=nand_info->partition.partition;
	for(i = 0; i < ptcount; i++){
		if(startaddr >= jz_mtd_spinand_partition[i].offset && (startaddr + length) <= (jz_mtd_spinand_partition[i].offset + jz_mtd_spinand_partition[i].size)){
			*pt_index = i;
			break;
		}
	}
	if(i >= ptcount){
		printf("startaddr 0x%x can't find the pt_index or you partition size 0x%x is not align with 128K\n",startaddr, length);
		*pt_index = -1;
		return NULL;
	}
	return &jz_mtd_spinand_partition[i];
}

int32_t mtd_sfcnand_probe_burner()
{
	struct jz_sfcnand_burner_param *param = spi_args->flash_info;
	struct mtd_info *mtd = &nand_info[0];
	struct nand_chip *chip;

	int32_t ret;

	if(jz_sfc_nand_init()) {
		printf("ERR: jz_sfc_nand_init error!\n");
		return -EIO;
	}
	chip = mtd->priv;
	chip->scan_bbt(mtd);
	chip->options |= NAND_BBT_SCANNED;

	/* Normal 0: none 1, force-erase, force erase contain creat bbt*/
	/* If reserved partition is enable,
	 * 0: none,
	 * 1: force-erase without reserved partition,
	 * 2: force-erase with reserved partition,
	 * 3: force-erase with bad block,
	 * each force-erase contain creat bbt*/

	switch(spi_args->spi_erase){
		case 1:
#ifdef CONFIG_FLASH_RESERVED_PART
			if((ret = flash_erase_skip_reserved_part(flash, ((struct jz_sfcnand_flashinfo*)flash->flash_info)->partition.partition,
						((struct jz_sfcnand_flashinfo*)flash->flash_info)->partition.num_partition)))
				return ret;
			break;
		case 2:
#endif
			if((ret = run_command("nand erase.chip -y", 0)))
				return ret;
			break;
		case 3:
			if((ret = run_command("nand scrub.chip -y", 0)))
				return ret;
			break;
	}

	if(chip->bbt)
		free(chip->bbt);
	chip->scan_bbt(mtd);
	chip->options |= NAND_BBT_SCANNED;
	mtd_sfcnand_partition_analysis(mtd->erasesize, param->partition_num,
			(void *)&param->partition/*This is a structure rather than a pointer in burner*/);
	return 0;
}
#endif

#ifdef CONFIG_FLASH_RESERVED_PART
void nand_enable_reserved_part_writable()
{
	modify_reserved_part_rw_mode(flash, RESERVED_PART_RW);
}

void nand_disable_reserved_part_writable()
{
	modify_reserved_part_rw_mode(flash, RESERVED_PART_R_ONLY);
}
#endif

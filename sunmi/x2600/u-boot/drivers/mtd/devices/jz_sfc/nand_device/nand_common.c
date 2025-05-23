#include <asm/arch/sfc.h>
#include <asm/arch/spinand.h>
#include "../jz_sfc_common.h"

/*
 *	pageread_to_cache default:
 *	cmd = SPINAND_CMD_PARD
 *	addr_len = 3
 *	data_dummy_bits = 0
 *	dataen = disable
 *
 */
void nand_pageread_to_cache(struct sfc_transfer *transfer, struct flash_operation_message *op_info) {

	transfer->cmd_info.cmd = SPINAND_CMD_PARD;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = op_info->pageaddr;
	transfer->addr_len = 3;

	transfer->cmd_info.dataen = DISABLE;
	transfer->len = 0;

	transfer->data_dummy_bits = 0;
	transfer->ops_mode = CPU_OPS;
	return;
}
EXPORT_SYMBOL_GPL(nand_pageread_to_cache);

/*
 *  single read default format:
 *  cmd = SPINAND_CMD_FRCH
 *  addr_len = 2;
 *  data_dummy_bits = 8
 *  dataen = enable
 */
void nand_single_read(struct sfc_transfer *transfer, struct flash_operation_message *op_info) {

	transfer->cmd_info.cmd = SPINAND_CMD_FRCH;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = op_info->columnaddr;
	transfer->addr_len = 2;

	transfer->cmd_info.dataen = ENABLE;
	transfer->data = op_info->buffer;
	transfer->len = op_info->len;
	transfer->direction = GLB_TRAN_DIR_READ;

	transfer->data_dummy_bits = 8;
	transfer->ops_mode = CPU_OPS;
	return;
}
EXPORT_SYMBOL_GPL(nand_single_read);

/*
 *	quad read default format:
 *	cmd = SPINAND_CMD_RDCH_X4
 *  	addr_len = 2;
 *  	data_dummy_bits = 8;
 *  	dataen = enable
 */
void nand_quad_read(struct sfc_transfer *transfer, struct flash_operation_message *op_info) {

	transfer->cmd_info.cmd = SPINAND_CMD_RDCH_X4;
	transfer->sfc_mode = TM_QI_QO_SPI;

	transfer->addr = op_info->columnaddr;
	transfer->addr_len = 2;

	transfer->cmd_info.dataen = ENABLE;
	transfer->data = op_info->buffer;
	transfer->len = op_info->len;
	transfer->direction = GLB_TRAN_DIR_READ;

	transfer->data_dummy_bits = 8;
	transfer->ops_mode = CPU_OPS;
	return;
}
EXPORT_SYMBOL_GPL(nand_quad_read);

/*
 *	write_enable default:
 *	cmd = SPINAND_CMD_WREN
 *	addr = 0
 *	dummy = 0
 *	dataen = 0
 */
void nand_write_enable(struct sfc_transfer *transfer, struct flash_operation_message *op_info) {

	transfer->cmd_info.cmd = SPINAND_CMD_WREN;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = 0;
	transfer->addr_len = 0;

	transfer->cmd_info.dataen = DISABLE;
	transfer->len = 0;

	transfer->data_dummy_bits = 0;
	transfer->ops_mode = CPU_OPS;
	return;
}
EXPORT_SYMBOL_GPL(nand_write_enable);

/*
 *	single_load default:
 *	cmd = SPINAND_CMD_PRO_LOAD
 *	addr_len = 2
 *	data_dummy_bits = 0
 *	dataen = enable
 *
 */
void nand_single_load(struct sfc_transfer *transfer, struct flash_operation_message *op_info) {

	transfer->cmd_info.cmd = SPINAND_CMD_PRO_LOAD;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = op_info->columnaddr;
	transfer->addr_len = 2;

	transfer->cmd_info.dataen = ENABLE;
	transfer->data = op_info->buffer;
	transfer->len = op_info->len;
	transfer->direction = GLB_TRAN_DIR_WRITE;

	transfer->data_dummy_bits = 0;
	transfer->ops_mode = CPU_OPS;

}
EXPORT_SYMBOL_GPL(nand_single_load);

/*
 *	quad_load default:
 *  	cmd = SPINAND_CMD_PRO_LOAD_X4
 *  	addr_len = 2
 *  	data_dummy_bits = 0
 *  	dataen = enable
 */
void nand_quad_load(struct sfc_transfer *transfer, struct flash_operation_message *op_info) {

	transfer->cmd_info.cmd = SPINAND_CMD_PRO_LOAD_X4;
	transfer->sfc_mode = TM_QI_QO_SPI;

	transfer->addr = op_info->columnaddr;
	transfer->addr_len = 2;

	transfer->cmd_info.dataen = ENABLE;
	transfer->data = op_info->buffer;
	transfer->len = op_info->len;
	transfer->direction = GLB_TRAN_DIR_WRITE;

	transfer->data_dummy_bits = 0;
	transfer->ops_mode = CPU_OPS;

}
EXPORT_SYMBOL_GPL(nand_quad_load);

/*
 *	program_exec default:
 *	cmd = SPINAND_CMD_PRO_EN
 *	addr_len = 3
 *	data_dummy_bits = 0
 *	dataen = disable
 */
void nand_program_exec(struct sfc_transfer *transfer, struct flash_operation_message *op_info) {

	transfer->cmd_info.cmd = SPINAND_CMD_PRO_EN;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = op_info->pageaddr;
	transfer->addr_len = 3;

	transfer->cmd_info.dataen = DISABLE;
	transfer->len = 0;

	transfer->data_dummy_bits = 0;
	transfer->ops_mode = CPU_OPS;
}
EXPORT_SYMBOL_GPL(nand_program_exec);

/*
 *  get_program_feature default:
 *  cmd = SPINAND_CMD_GET_FEATURE
 *  addr = SPINAND_ADDR_STATUS
 *  data_dummy_bits = 0
 *  dataen = enable
 */
int32_t nand_get_program_feature(struct flash_operation_message *op_info) {

	struct sfc_flash *flash = op_info->flash;
	struct sfc_transfer transfer;
	uint32_t ecc_status;

retry:
	ecc_status = 0;
	memset(&transfer, 0, sizeof(transfer));
	sfc_list_init(&transfer);

	transfer.cmd_info.cmd = SPINAND_CMD_GET_FEATURE;
	transfer.sfc_mode = TM_STD_SPI;

	transfer.addr = SPINAND_ADDR_STATUS;
	transfer.addr_len = 1;

	transfer.cmd_info.dataen = ENABLE;
	transfer.data = (uint8_t *)&ecc_status;
	transfer.len = 1;
	transfer.direction = GLB_TRAN_DIR_READ;

	transfer.data_dummy_bits = 0;
	transfer.ops_mode = CPU_OPS;

	if(sfc_sync(flash->sfc, &transfer)) {
	        printf("sfc_sync error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}

	if(ecc_status & SPINAND_IS_BUSY)
		goto retry;
	if(ecc_status & (1 << 3))
		return -EIO;

	return 0;
}
EXPORT_SYMBOL_GPL(nand_get_program_feature);

/*
 *  block_erase default:
 *  cmd = SPINAND_CMD_ERASE_128K
 *  addr_len = 3
 *  data_dummy_bits = 0
 *  dataen = disable
 */
void nand_block_erase(struct sfc_transfer *transfer, struct flash_operation_message *op_info) {

	transfer->cmd_info.cmd = SPINAND_CMD_ERASE_128K;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = op_info->pageaddr;
	transfer->addr_len = 3;

	transfer->cmd_info.dataen = DISABLE;
	transfer->len = 0;

	transfer->data_dummy_bits = 0;
	transfer->ops_mode = CPU_OPS;
	return;
}
EXPORT_SYMBOL_GPL(nand_block_erase);

/*
 *	get_erase_feature default:
 *	cmd = SPINAND_CMD_GET_FEATURE
 *	addr_len = 1
 *	data_dummy_bits = 0
 *	dataen = disable
 *	polling = SPINAND_IS_BUSY
 */
int32_t nand_get_erase_feature(struct flash_operation_message *op_info) {

	struct sfc_flash *flash = op_info->flash;
	struct sfc_transfer transfer;
	uint32_t ecc_status;

retry:
	ecc_status = 0;
	memset(&transfer, 0, sizeof(transfer));
	sfc_list_init(&transfer);

	transfer.cmd_info.cmd = SPINAND_CMD_GET_FEATURE;
	transfer.sfc_mode = TM_STD_SPI;

	transfer.addr = SPINAND_ADDR_STATUS;
	transfer.addr_len = 1;

	transfer.cmd_info.dataen = ENABLE;
	transfer.data = (uint8_t *)&ecc_status;
	transfer.len = 1;
	transfer.direction = GLB_TRAN_DIR_READ;

	transfer.data_dummy_bits = 0;
	transfer.ops_mode = CPU_OPS;

	if(sfc_sync(flash->sfc, &transfer)) {
	        printf("sfc_sync error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}

	if(ecc_status & SPINAND_IS_BUSY)
		goto retry;

	if(ecc_status & (1 << 2))
		return -EIO;

	return 0;
}
EXPORT_SYMBOL_GPL(nand_get_erase_feature);

/*
 *	nand_set_feature default:
 *	cmd = SPINAND_CMD_SET_FEATURE
 *	addr_len = 1
 *	data_dummy_bits = 0
 *	dataen = enable
 *
 */
void nand_set_feature(struct sfc_transfer *transfer, uint8_t addr, uint32_t *val) {

	transfer->cmd_info.cmd = SPINAND_CMD_SET_FEATURE;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = addr;
	transfer->addr_len = 1;

	transfer->cmd_info.dataen = ENABLE;
	transfer->data = (uint8_t *)val;
	transfer->len = 1;
	transfer->direction = GLB_TRAN_DIR_WRITE;

	transfer->data_dummy_bits = 0;
	transfer->ops_mode = CPU_OPS;
	return;
}
EXPORT_SYMBOL_GPL(nand_set_feature);

/*
 *	nand_get_feature default:
 *	cmd = SPINAND_CMD_GET_FEATURE
 *	addr_len = 1
 *	data_dummy_bits = 0
 *	dataen = enable
 *
 */
void nand_get_feature(struct sfc_transfer *transfer, uint8_t addr, uint8_t *val) {

	transfer->cmd_info.cmd = SPINAND_CMD_GET_FEATURE;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = addr;
	transfer->addr_len = 1;

	transfer->cmd_info.dataen = ENABLE;
	transfer->data = val;
	transfer->len = 1;
	transfer->direction = GLB_TRAN_DIR_READ;

	transfer->data_dummy_bits = 0;
	transfer->ops_mode = CPU_OPS;
	return;
}
EXPORT_SYMBOL_GPL(nand_get_feature);

int32_t nand_get_ecc_conf(struct sfc_flash *flash, uint8_t addr)
{
	struct sfc_transfer transfer;
	uint32_t buf = 0;

	memset(&transfer, 0, sizeof(transfer));

	transfer.cmd_info.cmd = SPINAND_CMD_GET_FEATURE;
	transfer.sfc_mode = TM_STD_SPI;

	transfer.addr = addr;
	transfer.addr_len = 1;

	transfer.cmd_info.dataen = ENABLE;
	transfer.direction = GLB_TRAN_DIR_READ;
	transfer.data = (uint8_t *)&buf;

	transfer.data_dummy_bits = 0;
	transfer.ops_mode = CPU_OPS;

	if(sfc_sync(flash->sfc, &transfer)) {
		printf("sfc_sync error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}
	return buf;
}
EXPORT_SYMBOL_GPL(nand_get_ecc_conf);

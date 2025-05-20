#!/bin/bash
#
# hwinfo bin generate
# wangzhongping 2019.1.26
#

PROJECT_MODEL_NAME_DEFAULT="NT320"
PROJECT_VERIFY_KEY_DEFAULT="sunmi98ni9V68RMnjmIfSUNMIoCOMoh8jKZ1QWookjdm4ekql5kdnefh78knnis67JMSH3q3fv7jtNISARM0COMWWWNISHISTM32"
PROJECT_BUILD_DIR_DEFAULT="./"
PROJECT_WORK_DIR_DEFAULT="./"
TARGET_HWINFO_BIN_PATH="hwinfo.bin"
BLOCK_FILL_TOOL="block_fill"
#default value
HWINFO_MASK_DEFAULT="1234"
HWINFO_VERIFYKEY_MASK_DEFAULT="3075"
HWINFO_DATA_CRC=""
#HWINFO_EACH_BLCOK_LENS
HWINFO_BLOCK_LENS=128
# HWINFO_BLOCK_INDEX
HWINFO_BLOCK_PROJECT=0
HWINFO_BLOCK_PROJECT_KEY=93 
HWINFO_BLOCK_CRC_DATA=255
HWINFO_BLOCK_MAX=256
# DATA_SIZE
HWINFO_TOTAL_DATA_SIZE=`expr $HWINFO_BLOCK_LENS \* $HWINFO_BLOCK_MAX` #HWINFO_BLOCK_LENS * HWINFO_BLOCK_MAX
HWINFO_USER_DATA_SIZE=`expr $HWINFO_BLOCK_LENS \* $HWINFO_BLOCK_CRC_DATA` #HWINFO_BLOCK_LENS * HWINFO_BLOCK_CRC_DATA

#get model name
get_model_name()
{
	if [ "$PROJECT_MODEL_NAME" = "" ]
	then
		echo "get model name failed, use default: $PROJECT_MODEL_NAME_DEFAULT"
	else
		PROJECT_MODEL_NAME_DEFAULT=$PROJECT_MODEL_NAME
		echo "get model name success: $PROJECT_MODEL_NAME_DEFAULT"
	fi
}

#get targit build dir
get_build_dir()
{
	if [ "$TARGET_BUILD_DIR" = "" ]
	then
		echo "get targit build dir failed, use default: $PROJECT_BUILD_DIR_DEFAULT"
		TARGET_HWINFO_BIN_PATH=$PROJECT_BUILD_DIR_DEFAULT$TARGET_HWINFO_BIN_PATH
	else
		TARGET_HWINFO_BIN_PATH=$TARGET_BUILD_DIR$TARGET_HWINFO_BIN_PATH
		echo "get targit build dir success: $TARGET_BUILD_DIR"
	fi
}
#get targit build dir
get_work_dir()
{
	if [ "$HWINFO_WORK_DIR" = "" ]
	then
		echo "get targit build dir failed, use default: $PROJECT_WORK_DIR_DEFAULT"
		BLOCK_FILL_TOOL=$PROJECT_WORK_DIR_DEFAULT$BLOCK_FILL_TOOL
	else
		PROJECT_WORK_DIR_DEFAULT=$HWINFO_WORK_DIR
		BLOCK_FILL_TOOL=$PROJECT_WORK_DIR_DEFAULT$BLOCK_FILL_TOOL
		echo "get targit build dir success: $PROJECT_WORK_DIR_DEFAULT"
	fi
}

#hwinfo block fill tool generate
tool_gen()
{
	if [ -x $BLOCK_FILL_TOOL ]
	then
		echo "tool generate skip"
	else
		gcc $PROJECT_WORK_DIR_DEFAULT"tools.c" -o $PROJECT_WORK_DIR_DEFAULT"block_fill"
		ret=$?
		ret_ok=0
		if [ $ret -ne $ret_ok ]
		then
			echo "error: tool generate failed"
			return $ret
		else
			echo "tool generate success"
			return $ret_ok
		fi
	fi
}

#generate the user data empty bin, size:32768 -128
hwinfo_empty_bin_gen()
{
	dd if=/dev/zero of=$TARGET_HWINFO_BIN_PATH seek=0 bs=1 count=$HWINFO_USER_DATA_SIZE 2> /dev/null
	ret=$?
	ret_ok=0
	if [ $ret -ne $ret_ok ]
	then
		echo "error: hwinfo empty bin generate failed"
		return $ret
	else
		echo "hwinfo empty bin generate success"
		return $ret_ok
	fi
}

#fill the project model name
hwinfo_model_fill()
{
	$BLOCK_FILL_TOOL $TARGET_HWINFO_BIN_PATH $HWINFO_MASK_DEFAULT ${#PROJECT_MODEL_NAME_DEFAULT} $PROJECT_MODEL_NAME_DEFAULT $HWINFO_BLOCK_PROJECT
	ret=$?
	ret_ok=0
	if [ $ret -ne $ret_ok ]
	then
		echo "error: hwinfo model fill failed"
		return $ret
	else
		echo "hwinfo model fill success"
		return $ret_ok
	fi
}

#fill the flash  name
hwinfo_verifykey_fill()
{
	$BLOCK_FILL_TOOL $TARGET_HWINFO_BIN_PATH $HWINFO_VERIFYKEY_MASK_DEFAULT ${#PROJECT_VERIFY_KEY_DEFAULT} $PROJECT_VERIFY_KEY_DEFAULT $HWINFO_BLOCK_PROJECT_KEY
	ret=$?
	ret_ok=0
	if [ $ret -ne $ret_ok ]
	then
		echo "error: hwinfo verifykey fill failed"
		return $ret
	else
		echo "hwinfo verifykey fill success"
		return $ret_ok
	fi
}
#get the hwinfo data crc32
hwinfo_get_crc32()
{
	HWINFO_DATA_CRC=`crc32 $TARGET_HWINFO_BIN_PATH`
	ret=$?
	ret_ok=0
	if [ $ret -ne $ret_ok ]
	then
		echo "error: hwinfo get crc32 failed: $HWINFO_DATA_CRC"
		return $ret
	else
		echo "hwinfo get crc32 success: $HWINFO_DATA_CRC"
		return $ret_ok
	fi

}

#fill the hwinfo data crc32, from (32768-128) ~ 32768
hwinfo_crc32_fill()
{
	#add the crc block
	dd if=/dev/zero of=$TARGET_HWINFO_BIN_PATH seek=$HWINFO_USER_DATA_SIZE bs=1 count=$HWINFO_BLOCK_LENS 2> /dev/null
	ret=$?
	ret_ok=0
	if [ $ret -ne $ret_ok ]
	then
		echo "error: hwinfo crc dd failed"
		return $ret
	else
		echo "hwinfo crc dd success"
	fi

	$BLOCK_FILL_TOOL $TARGET_HWINFO_BIN_PATH $HWINFO_MASK_DEFAULT ${#HWINFO_DATA_CRC} $HWINFO_DATA_CRC $HWINFO_BLOCK_CRC_DATA
	ret=$?
	ret_ok=0
	if [ $ret -ne $ret_ok ]
	then
		echo "error: hwinfo crc fill failed"
		return $ret
	else
		echo "hwinfo crc fill success"
		return $ret_ok
	fi
}

echo
echo "hwinfo.bin make start"

get_model_name
get_build_dir
get_work_dir
tool_gen
if [ $? -ne 0 ]; then exit $ret; fi #if error,exit with errorno
hwinfo_empty_bin_gen
if [ $? -ne 0 ]; then exit $ret; fi #if error,exit with errorno
hwinfo_model_fill
if [ $? -ne 0 ]; then exit $ret; fi #if error,exit with errorno
hwinfo_verifykey_fill
if [ $? -ne 0 ]; then exit $ret; fi #if error,exit with errorno
hwinfo_get_crc32
if [ $? -ne 0 ]; then exit $ret; fi #if error,exit with errorno
hwinfo_crc32_fill
if [ $? -ne 0 ]; then exit $ret; fi #if error,exit with errorno

echo "hwinfo.bin make end"
echo




